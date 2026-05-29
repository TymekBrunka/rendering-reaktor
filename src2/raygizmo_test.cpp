/***************************************************************************************************
 *
 *   LICENSE: zlib
 *
 *   Copyright (c) 2025 Claudio Z. (@cloudofoz)
 *
 *   This software is provided "as-is," without any express or implied warranty. In no event
 *   will the authors be held liable for any damages arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose, including commercial
 *   applications, and to alter and redistribute it freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not claim that you
 *        wrote the original software. If you use this software in a product, an acknowledgment
 *        in the product documentation would be appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such and must not be misrepresented
 *        as being the original software.
 *
 *     3. This notice may not be removed or altered from any source distribution.
 *
 ***************************************************************************************************/

/*
 * Example 03 - Gizmo With Render Targets
 * Demonstrates four independent 3D viewports, each rendered to its own texture.
 * The main gizmo (translate mode) is active in the viewport currently under the cursor.
 */

#include "raylib.h"
#include "raygizmo.h"
#include "raymath.h"

//--------------------------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------------------------
#define SCREEN_WIDTH   960
#define SCREEN_HEIGHT  540
#define VIEWPORT_COUNT 4

static const char* EXAMPLE_TITLE = "Example 03 - Gizmo With Render Targets";

static Vector2 lookRotation = {0};
static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = 1.0f;
static Vector2 sensitivity = {0.003f, 0.003f};
static Vector2 lean = {0};

static void UpdateCameraFPS(Camera *camera) {
  const Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = (Vector3){0.0f, 0.0f, -1.0f};

  // Left and right
  Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

  // Clamp view up
  float maxAngleUp = Vector3Angle(up, yaw);
  maxAngleUp -= 0.001f; // Avoid numerical errors
  if (-(lookRotation.y) > maxAngleUp) {
    lookRotation.y = -maxAngleUp;
  }

  // Clamp view down
  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;  // Downwards angle is negative
  maxAngleDown += 0.001f; // Avoid numerical errors
  if (-(lookRotation.y) < maxAngleDown) {
    lookRotation.y = -maxAngleDown;
  }

  // Up and down
  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  // Rotate view vector around right axis
  float pitchAngle = -lookRotation.y - lean.y;
  pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  // Head animation
  // Rotate up direction around forward axis
  float headSin = sinf(headTimer * PI);
  float headCos = cosf(headTimer * PI);
  const float stepRotation = 0.01f;
  camera->up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

  // Camera BOB
  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  camera->position = Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
  camera->target = Vector3Add(camera->position, pitch);
}

//--------------------------------------------------------------------------------------------------
// Types
//--------------------------------------------------------------------------------------------------
typedef struct
{
	Rectangle area; // Screen rectangle
	RenderTexture rt; // Render target
	Camera camera; // Per-viewport camera
	Color clearColor; // Background color
} Viewport;

//--------------------------------------------------------------------------------------------------
// Local Helpers
//--------------------------------------------------------------------------------------------------
static Camera CreateCamera(Vector3 position, Vector3 target)
{
	Camera cam;
	cam.position = position;
	cam.target = target;
	cam.up = (Vector3){0.0f, 1.0f, 0.0f};
	cam.fovy = 45.0f;
	cam.projection = CAMERA_PERSPECTIVE;
	return cam;
}

//--------------------------------------------------------------------------------------------------
// Program Entry
//--------------------------------------------------------------------------------------------------
int main(void)
{
	//----------------------------------------------------------------------------------------------
	// Initialization
	//----------------------------------------------------------------------------------------------
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
	           TextFormat("raylib-gizmo | %s", EXAMPLE_TITLE));
	SetTargetFPS(60);

	// Load assets
	Texture crateTexture = LoadTexture("resources/textures/crate_texture.jpg");
	GenTextureMipmaps(&crateTexture);
	SetTextureFilter(crateTexture, TEXTURE_FILTER_TRILINEAR);

	Model crateModel = LoadModel("resources/models/crate_model.obj");
	crateModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = crateTexture;

	// Initial transform for the crate (will be modified by the gizmo)
	Transform crateTransform = GizmoIdentity();

	// Viewport configuration
	const Color BACK_COLORS[VIEWPORT_COUNT] = {BLACK, PURPLE, ORANGE, RED};
	const Vector3 CAM_POSITIONS[VIEWPORT_COUNT] =
	{
		{-5.5f, 5.5f, 2.0f},
		{5.5f, 5.5f, 2.0f},
		{-2.5f, 2.5f, 2.0f},
		{2.5f, 2.5f, 2.0f}
	};

	const float HALF_W = (float)SCREEN_WIDTH * 0.5f;
	const float HALF_H = (float)SCREEN_HEIGHT * 0.5f;

	Viewport view[VIEWPORT_COUNT];
	int i;
	for (i = 0; i < VIEWPORT_COUNT; ++i)
	{
		const float col = (float)(i % 2);
		const float row = (float)(i / 2);

		view[i].area = (Rectangle){col * HALF_W, row * HALF_H, HALF_W, HALF_H};
		view[i].clearColor = BACK_COLORS[i];
		view[i].camera = CreateCamera(CAM_POSITIONS[i], (Vector3){0.0f, 0.0f, 0.0f});
		view[i].rt = LoadRenderTexture((int)HALF_W, (int)HALF_H);
	}

	// Increase the gizmo size for better visibility
	SetGizmoSize(4.0f);

	//----------------------------------------------------------------------------------------------
	// Main Loop
	//----------------------------------------------------------------------------------------------
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground((Color){0, 0, 25, 255});

    Vector2 mouseDelta = GetMouseDelta();
    lookRotation.x -= mouseDelta.x * sensitivity.x;
    lookRotation.y += mouseDelta.y * sensitivity.y;

		for (i = 0; i < VIEWPORT_COUNT; ++i)
		{
			UpdateCameraFPS(&view[i].camera);
			// Render the scene to the viewport's render target
			BeginTextureMode(view[i].rt);
			ClearBackground(view[i].clearColor);
			BeginMode3D(view[i].camera);

			crateModel.transform = GizmoToMatrix(crateTransform);
			DrawModel(crateModel, Vector3Zero(), 1.0f, WHITE);

			// Activate gizmo if mouse is inside this viewport
			if (CheckCollisionPointRec(GetMousePosition(), view[i].area))
			{
				SetMouseOffset(-(int)view[i].area.x, -(int)view[i].area.y);
				SetMouseScale((float)view[i].rt.texture.width / view[i].area.width,
				              (float)view[i].rt.texture.height / view[i].area.height);

				DrawGizmo3D(GIZMO_ALL, &crateTransform);

				SetMouseOffset(0, 0);
				SetMouseScale(1.0f, 1.0f);
			}

			EndMode3D();
			EndTextureMode();

			// Draw the render target to the window
			DrawTextureRec(view[i].rt.texture,
			               (Rectangle){
				               0, 0,
				               (float)view[i].rt.texture.width,
				               -(float)view[i].rt.texture.height
			               },
			               (Vector2){
				               view[i].area.x, view[i].area.y
			               },
			               WHITE);
		}

		EndDrawing();
	}

	//----------------------------------------------------------------------------------------------
	// Cleanup
	//----------------------------------------------------------------------------------------------
	for (i = 0; i < VIEWPORT_COUNT; ++i) UnloadRenderTexture(view[i].rt);
	UnloadTexture(crateTexture);
	UnloadModel(crateModel);
	CloseWindow();

	return 0;
}
