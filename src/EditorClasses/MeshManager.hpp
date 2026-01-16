#pragma once
#include "VertexBuffer.hpp"
#include "WorkerThreads.hpp"
#include "generators.h"
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MeshManager {

struct Mesh_vertex {
  RR::vec3 pos;
  RR::vec2 uv;
  RR::vec3 normal;
};

struct Instance_data {
  int id;
  RR::vec4 _id;
  glm::mat4 transform;
};

struct AwaitingMesh {
};

inline std::vector<RR::VertexBuffer<Mesh_vertex>> meshes;
inline std::vector<Instance_data> instance_data;

void load_from_file(std::string filepath);
void render_thread_post_work(worker_status status);

} // namespace MeshManager
