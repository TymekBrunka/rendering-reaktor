#include "MeshManager.hpp"
#include "VertexBuffer.hpp"
#include "utils/Logger.hpp"

namespace MeshManager {

AwaitingMesh *load_from_file(std::string filepath) {
  tinyobj::ObjReaderConfig reader_config;
  // reader_config.mtl_search_path = "./"; // Path to material files
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(filepath, reader_config)) {
    if (!reader.Error().empty()) {
      Logger<>::error("OBJ LOADER") << "TinyObjReader: " << reader.Error();
    }
    return nullptr;
  }

  if (!reader.Warning().empty()) {
    Logger<>::warn("OBJ LOADER") << "TinyObjReader: " << reader.Warning();
  }

  AwaitingMesh *am = new AwaitingMesh{};

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  float vx, vy, vz, nx, ny, nz, tx, ty;

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
        // Check if `normal_index` is zero or positive. negative = no normal data
        if (idx.normal_index >= 0) {
          nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
        }
        // Check if `texcoord_index` is zero or positive. negative = no texcoord data
        if (idx.texcoord_index >= 0) {
          tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
        }
        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

        am->vertex_data.push_back({
            .pos = {vx, vy, vz},
            .uv = {tx, ty},
            .normal = {nx, ny, nz},
        });
      }

      index_offset += fv;

      // // per-face material
      // shapes[s].mesh.material_ids[f];
    }
  }

  return am;
}

void render_thread_post_work(worker_status status) {
  std::lock_guard lg(awaiting_meshes_mutex);

  for (auto &am : awaiting_meshes) {
    RR::VertexBuffer<Mesh_vertex> mesh(am->vertex_data.data(), am->vertex_data.size(), GL_STATIC_DRAW);
    meshes.push_back(std::move(mesh));
    delete am;
  }
}

} // namespace MeshManager
