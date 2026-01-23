#include "MeshManager.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "utils/Logger.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
// #include "tiny_obj_loader.h"

namespace MeshManager {

template <typename T> void printvec(std::vector<T> &vec) {
  std::cout << "Vector contents: \n";
  for (auto &i : vec) {
    std::cout << i;
  }
  std::cout << "\n";
}

std::ostream &operator<<(std::ostream &stream, Mesh_vertex vertex) {
  stream << "pos: " << vertex.pos[0] << " , " << vertex.pos[1] << " , " << vertex.pos[2] << "\n";
  stream << "uv: " << vertex.uv[0] << " , " << vertex.uv[1] << "\n";
  stream << "normal: " << vertex.normal[0] << " , " << vertex.normal[1] << " , " << vertex.normal[2] << "\n";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, Mesh& mesh) {
  stream << "mesh (vb, ib): " << mesh.vb.id << " , " << mesh.ib.id << "\n";
  return stream;
}

void vertexArraySetup(RR::Program &program) {
  // clang-format off
  vertex_array = RR::VertexArray("");
  vertex_array.setStructure(program, sizeof(Mesh_vertex), {
    {"pos", RR::AttribKind::VEC3, GL_FALSE, offsetof(Mesh_vertex, pos)},
    {"uv", RR::AttribKind::VEC2, GL_TRUE, offsetof(Mesh_vertex, uv)},
    {"normal", RR::AttribKind::VEC3, GL_TRUE, offsetof(Mesh_vertex, normal)},
  });
  // clang-format on
}

void openDialogAndLoad() {
  pfd::open_file f = pfd::open_file("Wybierz plik z modelem 3D", pfd::path::home(), {"Modele 3D (.obj)", "*.obj", "Wszystkie pliki", "*"}, pfd::opt::multiselect);
  std::vector<MeshManager::AwaitingMesh *> am_s;
  for (auto const &name : f.result()) {
    MeshManager::AwaitingMesh *am = MeshManager::load_from_file(name);
    if (am != nullptr) {
      am_s.push_back(am);
    }
  }
  {
    std::lock_guard lg(MeshManager::awaiting_meshes_mutex);
    for (auto &am_ : am_s) {
      MeshManager::awaiting_meshes.push_back(am_);
    }
    std::cout << "broke free\n";
  }
}

// AwaitingMesh *load_from_file(std::string filepath) {
//   tinyobj::ObjReaderConfig reader_config;
//   // reader_config.mtl_search_path = "./"; // Path to material files
//   reader_config.triangulate = true;
//   tinyobj::ObjReader reader;
//
//   if (!reader.ParseFromFile(filepath, reader_config)) {
//     if (!reader.Error().empty()) {
//       Logger<>::error("OBJ LOADER") << "TinyObjReader: " << reader.Error();
//     }
//     return nullptr;
//   }
//
//   if (!reader.Warning().empty()) {
//     Logger<>::warn("OBJ LOADER") << "TinyObjReader: " << reader.Warning();
//   }
//
//   AwaitingMesh *am = new AwaitingMesh{};
//
//   auto &attrib = reader.GetAttrib();
//   auto &shapes = reader.GetShapes();
//   auto &materials = reader.GetMaterials();
//
//   float vx, vy, vz, nx, ny, nz, tx, ty;
//
//   // Loop over shapes
//   for (size_t s = 0; s < shapes.size(); s++) {
//     // Loop over faces(polygon)
//     size_t index_offset = 0;
//     for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
//       size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
//       // Loop over vertices in the face.
//       for (size_t v = 0; v < fv; v++) {
//         // access to vertex
//         tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//         vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
//         vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
//         vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
//         // Check if `normal_index` is zero or positive. negative = no normal data
//         if (idx.normal_index >= 0) {
//           nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
//           ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
//           nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
//         }
//         // Check if `texcoord_index` is zero or positive. negative = no texcoord data
//         if (idx.texcoord_index >= 0) {
//           tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
//           ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
//         }
//         // Optional: vertex colors
//         // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
//         // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
//         // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
//
//         am->vertex_data.push_back({
//             .pos = {vx, vy, vz},
//             .uv = {tx, ty},
//             .normal = {nx, ny, nz},
//         });
//       }
//
//       index_offset += fv;
//
//       // // per-face material
//       // shapes[s].mesh.material_ids[f];
//     }
//   }
//
//   printvec(am->vertex_data);
//   return am;
// }

void processMesh(aiMesh *mesh, AwaitingMesh *am) {
  Mesh_vertex mv;
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    // process vertex positions, normals and texture coordinates
    mv.pos[0] = mesh->mVertices[i].x;
    mv.pos[1] = mesh->mVertices[i].y;
    mv.pos[2] = mesh->mVertices[i].z;

    mv.normal[0] = mesh->mNormals[i].x;
    mv.normal[1] = mesh->mNormals[i].y;
    mv.normal[2] = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0]) {
      mv.uv[0] = mesh->mTextureCoords[0][i].x;
      mv.uv[1] = mesh->mTextureCoords[0][i].y;
    } else {
      mv.uv[0] = 0;
      mv.uv[1] = 0;
    }

    am->vertex_data.push_back(mv);
  }

  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      am->indices.push_back(face.mIndices[j]);
  }
}

// // process material
// if (mesh->mMaterialIndex >= 0) {
// }

void processNode(aiNode *node, const aiScene *scene, AwaitingMesh *am) {
  // process all the node's meshes (if any)
  std::cout << "ive been called\n";
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, am);
  }

  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, am);
  }
}

AwaitingMesh *load_from_file(std::string filepath) {
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    Logger<>::error("ASSIMP") << importer.GetErrorString() << "\n";
    return nullptr;
  }

  AwaitingMesh *am = new AwaitingMesh{};
  processNode(scene->mRootNode, scene, am);
  // printvec(am->vertex_data);
  std::cout << "done\n";
  return am;
}

void render_thread_post_work(worker_status status) {
  std::lock_guard lg(awaiting_meshes_mutex);

  for (auto &am : awaiting_meshes) {
    RR::VertexBuffer<Mesh_vertex> mesh(am->vertex_data.data(), am->vertex_data.size(), GL_STATIC_DRAW);
    RR::IndexBuffer mesh_i(am->indices.data(), am->indices.size(), GL_STATIC_DRAW);
    std::cout << "ids: " << mesh.id << " , " << mesh_i.id << "\n";
    std::cout << "meshes: " << meshes.size() << "\n";
    meshes.push_back({std::move(mesh), std::move(mesh_i)});
    // printvec(meshes);
    delete am;
  }
  awaiting_meshes.clear();
}

} // namespace MeshManager
