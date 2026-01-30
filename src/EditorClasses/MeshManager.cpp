#include "MeshManager.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "imgui.h"
#include "utils/Logger.hpp"
#include <cstddef>
#include <glm/fwd.hpp>
#include <iostream>
#include <ostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
// #include "tiny_obj_loader.h"

#include "main.hpp"

#ifndef _WIN32
#define DIR_SEPARATOR '/'
#else
#define DIR_SEPARATOR '\\'
#endif

namespace MeshManager {

// template <typename T> void printvec(std::vector<T> &vec) {
//   std::cout << "Vector contents: \n";
//   for (auto &i : vec) {
//     std::cout << i;
//   }
//   std::cout << "\n";
// }
//
// std::ostream &operator<<(std::ostream &stream, Mesh_vertex vertex) {
//   stream << "pos: " << vertex.pos[0] << " , " << vertex.pos[1] << " , " << vertex.pos[2] << "\n";
//   stream << "uv: " << vertex.uv[0] << " , " << vertex.uv[1] << "\n";
//   stream << "normal: " << vertex.normal[0] << " , " << vertex.normal[1] << " , " << vertex.normal[2] << "\n";
//   return stream;
// }
//
// std::ostream &operator<<(std::ostream &stream, Mesh &mesh) {
//   stream << "mesh (vb, ib): " << mesh.vb.id << " , " << mesh.ib.id << "\n";
//   return stream;
// }

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
    // std::cout << "broke free\n";
  }
}

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
  // std::cout << "ive been called\n";
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

  int last_of_separator = filepath.find_last_of(DIR_SEPARATOR);
  int last_of_fwd_slash = filepath.find_last_of('/');

  if (last_of_fwd_slash > last_of_separator) last_of_separator = last_of_fwd_slash;

  AwaitingMesh *am = new AwaitingMesh{
    .name = filepath.substr(last_of_separator + 1)
  };
  processNode(scene->mRootNode, scene, am);
  // printvec(am->vertex_data);
  // std::cout << "done\n";
  return am;
}

void render_thread_post_work(worker_status status) {
  std::lock_guard lg(awaiting_meshes_mutex);

  glUseProgram(model_program.id);
  for (auto &am : awaiting_meshes) {
    RR::VertexBuffer<Mesh_vertex> mesh(am->vertex_data.data(), am->vertex_data.size(), GL_STATIC_DRAW);
    RR::IndexBuffer mesh_i(am->indices.data(), am->indices.size(), GL_STATIC_DRAW);
    RR::VertexArray va("");
    mesh.bind();
    mesh_i.bind();
    va.setStructure(model_program, sizeof(Mesh_vertex),
                    {
                        {"pos", RR::AttribKind::VEC3, GL_FALSE, offsetof(Mesh_vertex, pos)},
                        {"uv", RR::AttribKind::VEC2, GL_TRUE, offsetof(Mesh_vertex, uv)},
                        {"normal", RR::AttribKind::VEC3, GL_TRUE, offsetof(Mesh_vertex, normal)},
                    });
    // std::cout << "ids: " << mesh.id << " , " << mesh_i.id << std::endl;
    // std::cout << "meshes: " << meshes.size() << std::endl;
    meshes.push_back({std::move(am->name), std::move(va), std::move(mesh), std::move(mesh_i)});
    // std::cout << "pushed ids: " << meshes[0].vb.id << " , " << meshes[0].ib.id << std::endl;
    // printvec(am->vertex_data);
    delete am;
  }
  awaiting_meshes.clear();
}

void UI() {
  ImGui::Begin("Modele 3D", NULL);
  {
    if (ImGui::BeginListBox("##")) {
      for (auto &mesh : meshes) {
        if (ImGui::Selectable(mesh.name.c_str())) {
          Instance_data instance = {
            .id = _id,
            ._id = {
              ((float)(_id & 0x00FF0000)) * (1.0/256.0) * (1.0/256.0) * (1.0/256.0),
              ((float)(_id & 0x0000FF00)) * (1.0/256.0) * (1.0/256.0),
              ((float)(_id & 0x000000FF)) * (1.0/256.0),
              0
            },
            .mesh = mesh, // INFO: mesh is stored as a reference, migh couse issues if missused
            .transform = glm::mat4(1)
          };
          std::cout << "vec4 id: " \
            << ((float)(_id & 0x00FF0000)) * (1.0/256.0) * (1.0/256.0) * (1.0/256.0)  << ", " \
            << ((float)(_id & 0x0000FF00)) * (1.0/256.0) * (1.0/256.0)  << ", " \
            << ((float)(_id & 0x000000FF)) * (1.0/256.0)  << ", " \
            << 0.0 << "\n";
          _id++;
          if (instances.size() > 0) instances.pop_back(); // for sake of testing
          instances.push_back(instance);
        }
      }
      ImGui::EndListBox();
    }
  }
  ImGui::End();
}

} // namespace MeshManager
