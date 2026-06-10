#include "raylib.h"
#include <App.hpp>
#include <imgui.h>
#include <iostream>
#include <raymath.h>
#include <rlImGui.h>
#include <rlgl.h>

#include <embeded/cubemap.png.hpp>
#include <embeded/icon.png.hpp>
#include <embeded/icons.png.hpp>

void App::initialise() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "reaktory");

  Image icon_ = {
      .data = icon_data.data,
      .width = icon_data.width,
      .height = icon_data.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image icons_ = {
      .data = iconsImg.data,
      .width = iconsImg.width,
      .height = iconsImg.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image skybox_ = {
      .data = cubemapImg.data,
      .width = cubemapImg.width,
      .height = cubemapImg.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  SetWindowIcon(icon_);

  assets.icon = LoadTextureFromImage(icon_);
  assets.icons = LoadTextureFromImage(icons_);
  assets.skybox = LoadTextureFromImage(skybox_);

  cube = GenMeshCube(1, 1, 1);
  skybox = LoadModelFromMesh(cube);
  skybox.materialCount = 1;
  skybox.materials[0].maps->texture = assets.skybox;

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // BlendMode(BLEND_ALPHA);
  rlSetBlendMode(BLEND_ALPHA);

  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = Vector3{
      body.position.x,
      body.position.y + 1,
      body.position.z,
  };

  updateCamera();

  DisableCursor(); // Limit cursor to relative movement inside the window

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
}

void App::run() {
  while (!WindowShouldClose()) {

    Vector2 mouseDelta = GetMouseDelta();
    orientation.x -= mouseDelta.x * sensitivity.x;
    orientation.y += mouseDelta.y * sensitivity.y;
    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));

    updateBody();

    float delta = GetFrameTime();
    // headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f * delta);
    camera.position = Vector3{
        body.position.x,
        body.position.y + 1,
        body.position.z,
    };

    // if (body.isGrounded && ((forward != 0) || (sideway != 0))) {
    //   headTimer += delta * 3.0f;
    //   walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
    //   camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f * delta);
    // } else {
    walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
    camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f * delta);
    // }

    lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
    lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);

    updateCamera();

    BeginDrawing();
    ClearBackground(BLANK);

    render_scene();

    rlImGuiBegin();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0x5f151515)); // workaround to make docked windows transparent
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0.0f);
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    // panel_ui();

    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);

    ImGui::ShowDemoWindow();
    ImGui::ShowStyleEditor();
    rlImGuiEnd();

    EndDrawing();
  }
}

void App::updateCamera() {

  const Vector3 up = Vector3{0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = Vector3{0.0f, 0.0f, -1.0f};

  // Left and right
  Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, orientation.x);

  // Clamp view up
  float maxAngleUp = Vector3Angle(up, yaw);
  maxAngleUp -= 0.001f; // Avoid numerical errors
  if (-(orientation.y) > maxAngleUp) {
    orientation.y = -maxAngleUp;
  }

  // Clamp view down
  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;  // Downwards angle is negative
  maxAngleDown += 0.001f; // Avoid numerical errors
  if (-(orientation.y) < maxAngleDown) {
    orientation.y = -maxAngleDown;
  }

  // Up and down
  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  // Rotate view vector around right axis
  float pitchAngle = -orientation.y - lean.y;
  pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  // Head animation
  // Rotate up direction around forward axis
  float headSin = sinf(headTimer * PI);
  float headCos = cosf(headTimer * PI);
  const float stepRotation = 0.01f;
  camera.up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

  // Camera BOB
  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  camera.position = Vector3Add(camera.position, Vector3Scale(bobbing, walkLerp));
  camera.target = Vector3Add(camera.position, pitch);
}

void App::updateBody() {
  char side = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
  char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
  bool jumpPressed = IsKeyDown(KEY_SPACE);
  bool crouchHold = IsKeyDown(KEY_LEFT_SHIFT);
  Vector2 input = Vector2{(float)side, (float)-forward};

  float delta = GetFrameTime();

  if (jumpPressed) {
    body.velocity.y = 30.0f;
  } else if (crouchHold) {
    body.velocity.y = -30.0f;
  } else {
    body.velocity.y = 0.0f;
  }

  Vector3 front = Vector3{sinf(orientation.x), 0.f, cosf(orientation.x)};
  Vector3 right = Vector3{cosf(-orientation.x), 0.f, sinf(-orientation.x)};

  Vector3 desiredDir = Vector3{
      input.x * right.x + input.y * front.x,
      0.0f,
      input.x * right.z + input.y * front.z,
  };
  body.dir = Vector3Lerp(body.dir, desiredDir, CONTROL * delta);

  float decel = AIR_DRAG;
  Vector3 hvel = Vector3{body.velocity.x * decel, 0.0f, body.velocity.z * decel};

  float hvelLength = Vector3Length(hvel); // Magnitude
  if (hvelLength < (MAX_SPEED * 0.01f))
    hvel = Vector3{0};

  // This is what creates strafing
  float speed = Vector3DotProduct(hvel, body.dir);

  // Whenever the amount of acceleration to add is clamped by the maximum acceleration constant,
  // a Player can make the speed faster by bringing the direction closer to horizontal velocity angle
  // More info here: https://youtu.be/v3zT3Z5apaM?t=165
  float accel = Clamp(MAX_SPEED - speed, 0.f, MAX_ACCEL * delta);
  hvel.x += body.dir.x * accel;
  hvel.z += body.dir.z * accel;

  body.velocity.x = hvel.x;
  body.velocity.z = hvel.z;

  body.position.x += body.velocity.x * delta;
  body.position.y += body.velocity.y * delta;
  body.position.z += body.velocity.z * delta;
}

void App::panel_ui() {
  ImGui::SetNextWindowBgAlpha(0.5f);
  if (ImGui::Begin("ThePanel")) {
    rlImGuiImageSize(&assets.icon, 20, 20);
    ImGui::SameLine();
    ImGui::Text("reaktory");
  }
  ImGui::End();
}

void App::render_scene() {
  DrawText("Reaktory", 20, 20, 20, BLUE);
  DrawTexture(assets.icon, 30, 30, WHITE);

  BeginMode3D(camera);
  rlDisableBackfaceCulling();
  rlDisableDepthMask();
  DrawModel(skybox, camera.position, 1.0f, WHITE);
  rlEnableBackfaceCulling();
  rlEnableDepthMask();
  EndMode3D();

  BeginMode3D(camera);
  DrawCube({-10, -15, -20}, 20, 30, 40, RED);
  EndMode3D();
}

void App::cleanup() {
  rlImGuiShutdown();
  CloseWindow();
}
