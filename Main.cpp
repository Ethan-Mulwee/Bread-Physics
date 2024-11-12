#include <iostream>
#include "bMath/bMath.hpp"
#include "bMath/ext/raylib.hpp"
#include "bMath/ext/iostream.hpp"
#include "bEngine/rigidbody.hpp"
// #include "src/rigidbody.cpp"

// build command for when cmake dies: g++ Main.cpp -Iinclude/ -lraylib 

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
    block.body.inverseMass = (1/2.0f);
    block.body.inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));

    while(!WindowShouldClose()) {
        Ray screenRay = GetScreenToWorldRay(GetMousePosition(), camera);
        RayCollision collision = GetRayCollisionMesh(screenRay, block.model.meshes[0], block.model.transform);

        // block.body.addForce(bMath::float3(0,-9.8,0));
        // block.body.addTorque(bMath::float3(0.2,0.2,0));

        // block.body.addForceAtPoint(bMath::rotate(bMath::float3(0.1,0,0), block.body.orientation), bMath::rotate(bMath::float3(2,2,0), block.body.orientation));

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            block.body.addForceAtPoint(toBread(screenRay.direction), toBread(collision.point));
        std::cout << block.body.torqueAccum << "\n";
        block.body.integrate(1/60.0f);

        // std::cout << block.body.linearVelocity << "\n";

        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                block.render();
                if (collision.hit)
                    DrawSphere(collision.point, 0.1, PURPLE);
            EndMode3D();
        EndDrawing();
    }
}