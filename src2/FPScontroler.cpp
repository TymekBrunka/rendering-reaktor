#include <App.hpp>
#include <raylib.h>
#include <raymath.h>
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
    body.velocity.y = VERTICAL_SPEED;
  } else if (crouchHold) {
    body.velocity.y = -VERTICAL_SPEED;
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
