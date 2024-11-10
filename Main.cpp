#include <iostream>
#include "bMath/bMath.hpp"
#include <raylib.h>
#include "bEngine/rigidbody.hpp"

Camera camera;
const int axisLength = 4;

int main() {
    //Raylib stuff
    InitWindow(500,500,"test");

    camera.position = Vector3{5,5,-5};
    camera.target = Vector3{0,0,0};
    camera.up = Vector3{0,1,0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;


    while(!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);
            EndMode3D();
        EndDrawing();
    }
}