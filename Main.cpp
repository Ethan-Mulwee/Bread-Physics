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

    bMath::float3 bodyPoint;
    bMath::float3 dragPoint;
    bool dragging = false;

    bMath::float2 mouseDeltaPos;

    world.bodies[0].addForceAtBodyPoint(bMath::float3(0,-1,0), bMath::float3(1,1,1)*50);
    world.bodies[0].addForce(bMath::float3(0,-1,0)*30);
    world.bodies[1].addForce(bMath::float3(0,-1,0)*100);

    while(!WindowShouldClose()) {
        mouseDeltaPos = toBread(GetMouseDelta());

        for (int i = 0; i < world.bodies.size(); i++) {
          // world.bodies[i].addForce(bMath::float3(0,-9.8,0)*(1.0f/world.bodies[i].inverseMass));
        }

        world.bodies[1].angularVelocity = bMath::float3(1,0,0);
        world.step(1/60.0f);

        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                // block.render();
                // if (collision.hit)
                //     DrawSphere(collision.point, 0.1, PURPLE);
                // if (dragging) {
                //     DrawSphere(toRay(bodyPoint*body->getTransform()), 0.1,RED);
                //     DrawSphere(toRay(dragPoint), 0.1, ORANGE);
                //     DrawLine3D(toRay(bodyPoint*body->getTransform()), toRay(dragPoint), BLACK);
                // }
                renderer.render(world);
            EndMode3D();
        EndDrawing();
    }
}