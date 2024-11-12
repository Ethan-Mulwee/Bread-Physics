#include <iostream>
#include "bMath/bMath.hpp"
#include "bMath/ext/raylib.hpp"
// #include "bEngine/rigidbody.hpp"
#include "src/rigidbody.cpp"

Camera camera;
const int axisLength = 4;

struct Block {
    bEngine::rigidbody body;
    Model model;

    void render() {
        model.transform = toRay(body.getTransform());
        DrawModel(model, Vector3{0,0,0}, 1, WHITE);
    }
};

int main() {
    //Raylib stuff
    InitWindow(500,500,"test");
    SetTargetFPS(60);

    camera.position = Vector3{5,5,-5};
    camera.target = Vector3{0,0,0};
    camera.up = Vector3{0,1,0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    Block block; 
    block.model = LoadModelFromMesh(GenMeshCube(1,1,1));
    block.body.inverseMass = (1/10.0f);
    block.body.inverseInertiaTensor = bMath::inverse(bMath::IntertiaTensorCuboid(10,1,1,1));

    while(!WindowShouldClose()) {
        block.body.addForce(bMath::float3(0,-9.8,0));
        block.body.integrate(1/60.0f);

        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                block.render();
            EndMode3D();
        EndDrawing();
    }
}