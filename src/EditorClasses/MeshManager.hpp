#pragma once
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "WorkerThreads.hpp"
#include <iostream>
#include <vector>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glMathTypes.hpp"
#include "tiny_obj_loader.h"

namespace MeshManager {

struct Mesh_vertex {
  RR::vec3 pos;
  RR::vec2 uv;
  RR::vec3 normal;
};

static inline RR::VertexArray vertex_array;

struct Instance_data {
  int id;
  RR::vec4 _id;
  glm::mat4 transform;
};

struct AwaitingMesh {
  std::vector<Mesh_vertex> vertex_data;
};

inline std::vector<AwaitingMesh*> awaiting_meshes;
inline std::condition_variable cv;
inline std::mutex awaiting_meshes_mutex;

inline std::vector<RR::VertexBuffer<Mesh_vertex>> meshes;
inline std::vector<Instance_data> instance_data;

AwaitingMesh* load_from_file(std::string filepath);
void render_thread_post_work(worker_status status);
void openDialogAndLoad();

void vertexArraySetup(RR::Program& program);

} // namespace MeshManager
