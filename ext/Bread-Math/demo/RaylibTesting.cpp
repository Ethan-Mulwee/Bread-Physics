#include <raylib.h>
#include "bMath/bMath.hpp"
#include "bMath/ext/raylib.hpp"
#include "bMath/ext/iostream.hpp"

// g++ RaylibTesting.cpp -lraylib -I../include

Camera camera;
const float axisLength = 4;

void DrawAxes() {
  DrawGrid(5,1);
  DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
  DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
  DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);
}



int main() {
  InitWindow(750,750, "testing");
  camera.position = Vector3{5,5,-5};
  camera.target = Vector3{0,0,0};
  camera.up = Vector3{0,1,0};
  camera.fovy = 45;
  camera.projection = CAMERA_PERSPECTIVE;

  float points[500];
  bMath::float3 position(2,1,1);
  bMath::float3 velocity(.1f,0,0.2f);
  bMath::float3 acceleration;
  bMath::float3 a;
  float dt = 0.001f;

  while(!WindowShouldClose()) {
    if ((a-position).length() > 0.2f) {
      acceleration = bMath::normalized(a-position)*(1.0f/((a-position).length()*(a-position).length()));
    }
    else acceleration = bMath::float3();
    velocity += acceleration*dt;
    position += velocity*dt;

    UpdateCamera(&camera, CAMERA_ORBITAL);

    BeginDrawing();
    ClearBackground(Color{35,35,35,255});
    // DrawGraph(points, 500, 0, 500, 700, 300);
      BeginMode3D(camera);
        // DrawAxes();
        // drawVector(a, ORANGE);
        // drawVector(rotate(a,q), PURPLE);
        DrawSphere(ConvertRay(position), 0.3f, WHITE);
        DrawVector(bMath::float3(1,1,1), PURPLE, 3.0f);
        DrawViewport3D(10.0f, 10, 1.0f, 2.0f);
        DrawBasis(bMath::basisFromVector(bMath::float3(1,1,1)));
      EndMode3D();
    EndDrawing();
  }

}