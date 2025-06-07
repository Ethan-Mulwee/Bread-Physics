#include <raylib.h>
#include <imgui/imgui.h>
#include "../../lib/rlImGui.h"

#include <bMath/bMath.hpp>
#include <bMath/ext/raylib.hpp>

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT); 

  InitWindow(1920, 1080, "Test");
  rlImGuiSetup(true);
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  Camera camera;

  camera.position = Vector3{5,5,-5};
  camera.target = Vector3{0,0,0};
  camera.up = Vector3{0,1,0};
  camera.fovy = 45;
  camera.projection = CAMERA_PERSPECTIVE;

  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(Color{31,31,31,255});

      BeginMode3D(camera);
        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        DrawViewport3D(10.0f, 5, 1.0f, 1.0f);
      EndMode3D();

      rlImGuiBegin();
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        // ImGui::Begin("test", &open, 0);


        // ImGui::End();

      rlImGuiEnd();

    EndDrawing();
  }
}