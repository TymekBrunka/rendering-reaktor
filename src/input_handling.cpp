#include "rr.hpp"
#include <GLFW/glfw3.h>
#include "Camera.hpp"

RR::FrameBuffer sceneFb;
RR::FrameBuffer skyboxFb;
Camera camera(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec2(0.0f, 0.0f));
int Gwidth;
int Gheight;

float motion_input[] = {0, 0};

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_W:
      motion_input[1] += 1;
      break;
    case GLFW_KEY_S:
      motion_input[1] -= 1;
      break;
    case GLFW_KEY_A:
      motion_input[0] -= 1;
      break;
    case GLFW_KEY_D:
      motion_input[0] += 1;
      break;
    }
  } else if (action == GLFW_RELEASE) {
    switch (key) {
    case GLFW_KEY_W:
      motion_input[1] -= 1;
      break;
    case GLFW_KEY_S:
      motion_input[1] += 1;
      break;
    case GLFW_KEY_A:
      motion_input[0] += 1;
      break;
    case GLFW_KEY_D:
      motion_input[0] -= 1;
      break;
    }
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera.holding_rmb = true;
  } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    camera.holding_rmb = false;
  }
}

void mouse_callback(GLFWwindow *window, double x, double y) {
  glm::vec2 temp((float)x, (float)y);
  if (camera.holding_rmb) {
    // std::cout << x << ", " << y << "hai\n";
    camera.orientation -= glm::radians(temp - camera.last_mouse_pos) / 2.0f;
    camera.orientation = glm::vec2(glm::mod(camera.orientation.x, (2.0f * 3.14f)), glm::clamp(camera.orientation.y, (-0.5f * 3.14f) + 0.0001f, (0.5f * 3.14f) + 0.0001f));
    camera.update_view(camera.orientation);
    camera.computeMatricies();
  }
  camera.last_mouse_pos[0] = x;
  camera.last_mouse_pos[1] = y;
}

void APIENTRY gl_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
  // Some debug messages are just annoying informational messages
  switch (id) {
  case 131185: // glBufferData
    return;
  }

  Logger<>::error("OpenGL") << "\n / " << id << " Severity: ";

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH_ARB:
    std::cout << "\x1b[31mHigh\x1b[0m";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM_ARB:
    std::cout << "\x1b[33mMedium\x1b[0m";
    break;
  case GL_DEBUG_SEVERITY_LOW_ARB:
    std::cout << "\x1b[35mLow\x1b[0m";
    break;
    // // below doesnt work
    // case GL_DEBUG_SEVERITY_NOTIFICATION_ARB:
    //     std::cout << "Notification";
    //     break;
  }

  std::cout << " Type: ";

  switch (type) {
  case GL_DEBUG_TYPE_ERROR_ARB:
    std::cout << "\x1b[31mError\x1b[0m";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
    std::cout << "Deprecated Behavior";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
    std::cout << "Undefined Behavior";
    break;
  case GL_DEBUG_TYPE_PORTABILITY_ARB:
    std::cout << "Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE_ARB:
    std::cout << "Performance";
    break;
    // // below dont work
    // case GL_DEBUG_TYPE_MARKER:
    //     std::cout << "Marker";
    //     break;
    // case GL_DEBUG_TYPE_PUSH_GROUP:
    //     std::cout << "Push Group";
    //     break;
    // case GL_DEBUG_TYPE_POP_GROUP:
    //     std::cout << "Pop Group";
    //     break;
    // case GL_DEBUG_TYPE_OTHER:
    //     std::cout << "Other";
    //     break;
  }

  std::cout << " /\n > Source:  \x1b[34m";

  switch (source) {
  case GL_DEBUG_SOURCE_API_ARB:
    std::cout << "API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
    std::cout << "Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
    std::cout << "Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
    std::cout << "Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION_ARB:
    std::cout << "Application";
    break;
  case GL_DEBUG_SOURCE_OTHER_ARB:
    std::cout << "Other";
    break;
  }

  std::cout << "\x1b[0m\n > Message: " << message;

  std::cout << "\n\n";
}

static void window_size_callback(GLFWwindow *window, int width, int height) {
  Gwidth = width;
  Gheight = height;
  glfwGetFramebufferSize(window, &width, &height);
  // const float ratio = width / (float) height;
  glViewport(0, 0, width, height);
  sceneFb = RR::FrameBuffer(width, height, 2);
  skyboxFb = RR::FrameBuffer(width, height, 2);
  camera.update_projection(width, height, 120);
  camera.computeMatricies();
}
