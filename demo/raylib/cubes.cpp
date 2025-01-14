#include <iostream>
#include <bMath/bMath.hpp>
#include <bMath/ext/raylib.hpp>
#include <bMath/ext/iostream.hpp>
#include "bEngine/world.hpp"
#include "render.hpp"

Camera camera;
const int axisLength = 4;

int main() {
    //Raylib stuff
    InitWindow(500,500,"test");
    SetTargetFPS(200);

    camera.position = Vector3{5,5,-5};
    camera.target = Vector3{0,0,0};
    camera.up = Vector3{0,1,0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    bEngine::World world;
    Renderer renderer;

    // Cube1 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body = new bEngine::RigidBody();
    body->inverseMass = 0.5f;
    body->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body->position = bMath::float3(0,2,0);
    body->orientation = bMath::quaternion(0.951,0.189,0.198,-0.146);
    body->orientation.normalize();

    bEngine::Primitive* collider = new bEngine::Primitive();
    collider->type = bEngine::PrimitiveType::Cube;
    collider->height = 0.5f;
    collider->width = 0.5f;
    collider->offset = bMath::matrix4::identity();
    collider->body = body;

    world.bodies.push_back(body);
    world.colliders.push_back(collider);
    ///////////////////////////////////////////////////////////////////////////////////////

    // Cube2 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body2 = new bEngine::RigidBody();
    body2->inverseMass = 0.5f;
    body2->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body2->position = bMath::float3(2,2,0);
    body2->orientation = bMath::quaternion(1,0,0,0);
    body2->angularVelocity = bMath::float3(0,0,0);

    bEngine::Primitive* collider2 = new bEngine::Primitive();
    collider2->type = bEngine::PrimitiveType::Cube;
    collider2->height = 0.5f;
    collider2->width = 0.5f;
    collider2->offset = bMath::matrix4::identity();
    collider2->body = body2;

    world.bodies.push_back(body2);
    world.colliders.push_back(collider2);
    ///////////////////////////////////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {

        for (int i = 0; i < world.bodies.size(); i++) {
          world.bodies[i]->addForce(bMath::float3(0,-9.8,0)*(1.0f/world.bodies[i]->inverseMass));
        }


        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                world.step(1/200.0f);
                renderer.render(world);
            EndMode3D();
        EndDrawing();
    }
}