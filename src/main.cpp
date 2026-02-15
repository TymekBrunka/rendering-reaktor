#include "main.hpp"
#include "Camera.hpp"
#include "EditorActionsPanel.hpp"
#include "FrameBuffer.hpp"
#include "MeshManager.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "WorkerThreads.hpp"
#include "pfd/portable-file-dialogs.h"
#include "utils/Logger.hpp"

// #include "portable-file-dialogs.h"
#include "rr.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "composite.frag.glsl.hpp"
#include "composite.vertex.glsl.hpp"
#include "cubemap2.png.hpp"
#include "icon.png.hpp"
#include "icons.png.hpp"
#include "model.frag.glsl.hpp"
#include "model.vertex.glsl.hpp"
#include "skybox.frag.glsl.hpp"
#include "skybox.vertex.glsl.hpp"

#include "imgui.h"
#include "imgui_boilerplate.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "stb_image.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "input_handling.cpp"

struct skybox_vert {
  RR::vec3 pos;
  RR::vec2 uv;
};

struct composite_vert {
  RR::vec2 pos;
  RR::vec2 uv;
};

composite_vert composite_verticies[] = {{{-1.0, 1.0}, {0.0, 1.0}}, {{-1.0, -1.0}, {0.0, 0.0}}, {{1.0, -1.0}, {1.0, 0.0}}, {{-1.0, 1.0}, {0.0, 1.0}}, {{1.0, -1.0}, {1.0, 0.0}}, {{1.0, 1.0}, {1.0, 1.0}}};

RR::VertexBuffer<composite_vert> composite_vb;
RR::VertexArray composite_va;

GLint location_pos;
GLint location_uv;

std::string validateProgram(RR::Program &program) {
  glValidateProgram(program.id);
  int validation_status;
  glGetProgramiv(program.id, GL_VALIDATE_STATUS, &validation_status);

  std::string error_msg;
  if (!validation_status) {
    std::string shader_type_s;

    int msg_size = error_msg.size();
    error_msg.resize(512);
    glGetProgramInfoLog(program.id, 512, NULL, error_msg.data());
  }
  return error_msg;
}

int main() {

  // Check that a backend is available
  if (!pfd::settings::available()) {
    std::cout << "Portable File Dialogs are not available on this platform.\n";
    return 1;
  }

  // Set verbosity to true
  pfd::settings::verbose(true);

  RR::init();

  WorkerPool<5> workers_pool("");
  workers = &workers_pool;

  // imgui_boilerplate();
  GLFWwindow *window = RR::createWindow(800, 600, "Reaktory", 3, 3); // #version 330
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  glfwMakeContextCurrent(window); // context must be set first
  gladLoadGL();                   // only then we can load
  // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  if (GLAD_GL_ARB_debug_output) {
    Logger<>::info("MAIN") << "OpenGL debugging enabled\n";
    glDebugMessageCallbackARB(gl_debug, NULL);
  }
  //
  // GLint numExtensions;
  // glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
  // std::cout << "- Extensions" << std::endl;
  // for (GLint i = 0; i < numExtensions; i++) {
  //   std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
  // }

  glfwSwapInterval(1);

  {
    GLFWimage icon[1];
    icon[0].width = icon_data.width;
    icon[0].height = icon_data.height;
    icon[0].pixels = icon_data.data;
    glfwSetWindowIcon(window, 1, icon);
  }

  {
    RR::Texture2d icon(&icon_data);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    program = RR::Program("");
    RR::Shader skybox_vertex;
    RR::Shader skybox_fragment;

    model_program = RR::Program("");
    RR::Shader model_vertex;
    RR::Shader model_fragment;

    RR::Program composite_program("");
    RR::Shader composite_vertex;
    RR::Shader composite_fragment;
    try {
      skybox_vertex = RR::Shader(GL_VERTEX_SHADER, skybox_vertex_text);
      skybox_fragment = RR::Shader(GL_FRAGMENT_SHADER, skybox_fragment_text);
      program.attachShader(skybox_vertex).attachShader(skybox_fragment);
      program.link();

      model_vertex = RR::Shader(GL_VERTEX_SHADER, model_vertex_text);
      model_fragment = RR::Shader(GL_FRAGMENT_SHADER, model_fragment_text);
      model_program.attachShader(model_vertex).attachShader(model_fragment);
      model_program.link();

      composite_vertex = RR::Shader(GL_VERTEX_SHADER, composite_vertex_text);
      composite_fragment = RR::Shader(GL_FRAGMENT_SHADER, composite_fragment_text);
      composite_program.attachShader(composite_vertex).attachShader(composite_fragment);
      composite_program.link();
    } catch (std::string ex) {
      std::cout << ex << "\n";
      exit(1);
    }

    RR::Texture2d skyboxTexture(&cubemap2Img);
    RR::Texture2d icons(&iconsImg);

    // stbi_set_flip_vertically_on_load(true);
    // RR::image_data img = RR::readImage("src/icons.png", 4);
    // stbi_image_free(img.data);

    sceneFb = RR::FrameBuffer(800, 600, 2);
    skyboxFb = RR::FrameBuffer(800, 600, 1);

    glUseProgram(composite_program.id);

    const GLint location_skyboxTex = glGetUniformLocation(composite_program.id, "skybox");
    const GLint location_sceneTex = glGetUniformLocation(composite_program.id, "scene");
    glUniform1i(location_skyboxTex, 2);
    glUniform1i(location_sceneTex, 3);

    glUseProgram(model_program.id);

    const GLint location_texture = glGetUniformLocation(model_program.id, "tex");
    glUniform1i(location_texture, 1);

    const GLint location_id = glGetUniformLocation(model_program.id, "id");
    const GLint location_transforms = glGetUniformLocation(model_program.id, "transforms");

    // skyboxTexture.bindToSlot(0);
    const GLint world_camera = glGetUniformLocation(model_program.id, "world_camera");

    glUseProgram(program.id);

    skyboxTexture.bindToSlotAndName(program, 0, "skybox");
    const GLint skybox_camera = glGetUniformLocation(program.id, "skybox_camera");

    icon.bindToSlot(4);
    icons.bindToSlot(5);

    skybox_vert skybox_verticies[] = {
        // back
        {{-1.0, 1.0, -1.0}, {0.75, 0.665}},
        {{-1.0, -1.0, -1.0}, {0.75, 0.334}},
        {{1.0, -1.0, -1.0}, {1.00, 0.334}},
        {{1.0, 1.0, -1.0}, {1.00, 0.665}},

        // front
        {{1.0, 1.0, 1.0}, {0.25, 0.665}},
        {{1.0, -1.0, 1.0}, {0.25, 0.334}},
        {{-1.0, -1.0, 1.0}, {0.50, 0.334}},
        {{-1.0, 1.0, 1.0}, {0.50, 0.665}},

        // right
        {{1.0, 1.0, -1.0}, {0.00, 0.665}},
        {{1.0, -1.0, -1.0}, {0.00, 0.334}},
        {{1.0, -1.0, 1.0}, {0.25, 0.334}},
        {{1.0, 1.0, 1.0}, {0.25, 0.665}},

        // left
        {{-1.0, 1.0, 1.0}, {0.50, 0.665}},
        {{-1.0, -1.0, 1.0}, {0.50, 0.334}},
        {{-1.0, -1.0, -1.0}, {0.75, 0.334}},
        {{-1.0, 1.0, -1.0}, {0.75, 0.665}},

        // bottom
        {{-1.0, -1.0, -1.0}, {0.499, 0.000}},
        {{-1.0, -1.0, 1.0}, {0.499, 0.332}},
        {{1.0, -1.0, 1.0}, {0.251, 0.332}},
        {{1.0, -1.0, -1.0}, {0.251, 0.000}},

        // top
        {{-1.0, 1.0, 1.0}, {0.499, 0.667}},
        {{-1.0, 1.0, -1.0}, {0.499, 1.000}},
        {{1.0, 1.0, -1.0}, {0.251, 1.000}},
        {{1.0, 1.0, 1.0}, {0.251, 0.667}},
    };

    GLuint skybox_indecies[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

    composite_va = RR::VertexArray("");
    composite_vb = RR::VertexBuffer(composite_verticies, 6, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind ebo so vao doesnt point to it
    composite_va.setStructure(program, sizeof(composite_vert),
                              {
                                  {"pos", RR::AttribKind::VEC2, GL_FALSE, offsetof(composite_vert, pos)},
                                  {"uv", RR::AttribKind::VEC2, GL_TRUE, offsetof(composite_vert, uv)},
                              });

    RR::VertexArray skybox_va = RR::VertexArray("");
    RR::VertexBuffer<skybox_vert> skybox_vb(skybox_verticies, sizeof(skybox_verticies) / sizeof(skybox_vert), GL_STATIC_DRAW);
    RR::IndexBuffer skybox_ib(skybox_indecies, sizeof(skybox_indecies) / sizeof(GLuint), GL_STATIC_DRAW);

    skybox_va.setStructure(program, sizeof(skybox_vert),
                           {
                               {"pos", RR::AttribKind::VEC3, GL_TRUE, offsetof(skybox_vert, pos)},
                               {"uv", RR::AttribKind::VEC2, GL_TRUE, offsetof(skybox_vert, uv)},
                           });

    iminit(window, true);

    // Camera
    camera.update_projection(800, 600, 120);
    camera.computeMatricies();

    // const GLint skybox_camera = glGetUniformLocation(program.id, "skybox_camera");
    // const GLint world_camera = glGetUniformLocation(program.id, "world_camera");

    float currentFrame, lastFrame, deltaTime;

    glfwGetFramebufferSize(window, &Gwidth, &Gheight);
    // const float ratio = width / (float) height;
    glViewport(0, 0, Gwidth, Gheight);
    int cols;

    while (!glfwWindowShouldClose(window)) {
      workers->handle();
      currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      if (*(double *)motion_input) {
        camera.move(motion_input, deltaTime);
        camera.computeMatricies();
      }

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, skyboxFb.id);
      glViewport(0, 0, Gwidth, Gheight);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      imNewFrame();
      setupDocking(Gwidth, Gheight);

      glUseProgram(program.id);
      glUniformMatrix4fv(skybox_camera, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(camera.read().camera_skybox));

      skyboxTexture.bindToSlot(0);
      skybox_va.bind();
      skybox_vb.bind();
      skybox_ib.bind();

      // glDrawArrays(GL_TRIANGLES, 0, 6);
      glDrawElements(GL_TRIANGLES, sizeof(skybox_indecies) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

      // glClear(GL_DEPTH_BUFFER_BIT);
      glUseProgram(model_program.id);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sceneFb.id);
      glViewport(0, 0, Gwidth, Gheight);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUniformMatrix4fv(world_camera, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(camera.read().camera));

      for (auto &instance : MeshManager::instances) {
        instance.mesh.va.bind();
        instance.mesh.ib.bind();
        glUniform4fv(location_id, 1, (float *)&instance._id);
        glUniformMatrix4fv(location_transforms, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(glm::rotate(glm::mat4(1.0), lastFrame, glm::vec3(0.0, 1.0, 0.0))));
        glDrawElements(GL_TRIANGLES, instance.mesh.ib.length, GL_UNSIGNED_INT, 0);
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(composite_program.id);
      composite_va.bind();
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      skyboxFb.textures[0].bindToSlot(2);
      sceneFb.textures[0].bindToSlot(3);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      int old_size = ImGui::GetFont()->Scale;
      ImGui::GetFont()->Scale *= 0.9;
      EditorActionsPanel::UI(icon, icons);
      MeshManager::UI();
      ImGui::GetFont()->Scale = old_size;

      // ImGui::End();
      imrender();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glfwDestroyWindow(window);

  glfwTerminate();
}
