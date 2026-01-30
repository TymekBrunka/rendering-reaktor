#pragma once
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "WorkerThreads.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glMathTypes.hpp"

namespace MeshManager {

struct Mesh_vertex {
  RR::vec3 pos;
  RR::vec2 uv;
  RR::vec3 normal;
};

static inline int32_t _id = 1;

struct Mesh {
  const std::string name;
  RR::VertexArray va;
  RR::VertexBuffer<Mesh_vertex> vb;
  RR::IndexBuffer ib;
};

struct Instance_data {
  int id;
  RR::vec4 _id;
  Mesh& mesh;
  glm::mat4 transform;
};

struct AwaitingMesh {
  const std::string name;
  std::vector<Mesh_vertex> vertex_data;
  std::vector<GLuint> indices;
};

inline std::vector<AwaitingMesh*> awaiting_meshes;
inline std::condition_variable cv;
inline std::mutex awaiting_meshes_mutex;

inline std::vector<Mesh> meshes;
inline std::vector<Instance_data> instances;

AwaitingMesh* load_from_file(std::string filepath);
void render_thread_post_work(worker_status status);
void openDialogAndLoad();
void UI();

} // namespace MeshManager
