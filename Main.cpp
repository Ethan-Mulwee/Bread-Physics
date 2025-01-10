#include <iostream>
#include <bMath/bMath.hpp>
#include <bMath/ext/raylib.hpp>
#include <bMath/ext/iostream.hpp>
#include "bEngine/world.hpp"
#include "bEngine/render.hpp"

Camera camera;
const int axisLength = 4;

int main() {
    //Raylib stuff
    InitWindow(500,500,"test");
    SetTargetFPS(60);

    camera.position = Vector3{5,5,-5};
    camera.target = Vector3{0,0,0};
    camera.up = Vector3{0,1,0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    bEngine::World world;
    Renderer renderer;

    // Cube1 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody body;
    body.inverseMass = (1/2.0f);
    body.inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));

    bEngine::Primitive collider;
    collider.type = bEngine::PrimitiveType::Cube;
    collider.height = 0.5f;
    collider.width = 0.5f;
    collider.offset = bMath::matrix4::identity();

    body.collider = collider;
    world.bodies.push_back(body);
    ///////////////////////////////////////////////////////////////////////////////////////

    // Cube2 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody body2;
    body2.inverseMass = (1/2.0f);
    body2.inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body2.position = bMath::float3(2,2,2);

    bEngine::Primitive collider2;
    collider2.type = bEngine::PrimitiveType::Cube;
    collider2.height = 0.5f;
    collider2.width = 0.5f;
    collider2.offset = bMath::matrix4::identity();

    body2.collider = collider2;
    world.bodies.push_back(body2);
    ///////////////////////////////////////////////////////////////////////////////////////
    std::cout << "before: " << world.bodies[0].orientation.angle() << "\n";
    world.bodies[0].orientation = rotate2(bMath::quaternion(1,0,0,0), bMath::float3(100,0,0));
    std::cout << world.bodies[0].orientation << "\n";
    std::cout << "after: " << world.bodies[0].orientation.angle() << "\n";

    while(!WindowShouldClose()) {

        // world.bodies[0].addForceAtBodyPoint(bMath::float3(0,-1,0), bMath::float3(1,1,1));
        // world.bodies[1].addForceAtBodyPoint(bMath::float3(0,-.2,0), bMath::float3(0,1,0));
        world.step(1/60.0f);


        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                renderer.render(world);
            EndMode3D();
        EndDrawing();
    }
}