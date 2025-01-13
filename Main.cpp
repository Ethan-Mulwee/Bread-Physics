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
    SetTargetFPS(200);

    camera.position = Vector3{5,5,-5};
    camera.target = Vector3{0,0,0};
    camera.up = Vector3{0,1,0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    bEngine::World world;
    Renderer renderer;

    // Cube1 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody body;
    body.inverseMass = 0.5f;
    body.inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body.position = bMath::float3(0,2,0);
    body.orientation = bMath::quaternion(0.951,0.189,0.198,-0.146);
    body.orientation.normalize();

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
    body2.inverseMass = 0.5f;
    body2.inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body2.position = bMath::float3(2,2,0);
    body2.orientation = bMath::quaternion(1,0,0,0);
    body2.angularVelocity = bMath::float3(0,0,0);

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

    bool firstFrame = true;

    while(!WindowShouldClose()) {
        mouseDeltaPos = toBread(GetMouseDelta());

        Ray screenRay = GetScreenToWorldRay(GetMousePosition(), camera);
        RayCollision collision = GetRayCollisionMesh(screenRay, renderer.cubeModel.meshes[0], toRay(world.bodies[0].getTransform()));

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_CONTROL)) {
            if (collision.hit) {
                bodyPoint = world.bodies[0].positionToBodySpace((toBread(collision.point)));
                dragPoint = toBread(collision.point);
                dragging = true;
            }
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && !dragging) {
            if (collision.hit) {
                bodyPoint = world.bodies[0].positionToBodySpace((toBread(collision.point)));
                bMath::float3 worldSpaceBodyPoint = bodyPoint*world.bodies[0].getTransform();
                world.bodies[0].addForceAtPoint(toBread(collision.normal)*-4, worldSpaceBodyPoint);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
        }


        if (dragging) {
            bMath::float3 viewVector = bMath::normalized(toBread(camera.target) - toBread(camera.position));
            bMath::float3 tagentViewVector = bMath::normalized(bMath::cross(toBread(camera.up), viewVector));
            bMath::float3 cameraUpVector = bMath::cross(viewVector, tagentViewVector);
            bMath::matrix3 spaceRotation(
              tagentViewVector.x, cameraUpVector.x, viewVector.x,
              tagentViewVector.y, cameraUpVector.y, viewVector.y,
              tagentViewVector.z, cameraUpVector.z, viewVector.z
            );
            dragPoint += bMath::float3(-mouseDeltaPos.x*0.005, -mouseDeltaPos.y*0.005, 0)*spaceRotation;
            bMath::float3 worldSpaceBodyPoint = bodyPoint*world.bodies[0].getTransform();
            bMath::float3 force = dragPoint - worldSpaceBodyPoint;
            world.bodies[0].addForceAtPoint(force, worldSpaceBodyPoint);
        }

        for (int i = 0; i < world.bodies.size(); i++) {
          world.bodies[i].addForce(bMath::float3(0,-9.8,0)*(1.0f/world.bodies[i].inverseMass));
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
                if (dragging) {
                    DrawSphere(toRay(bodyPoint*world.bodies[0].getTransform()), 0.1,RED);
                    DrawSphere(toRay(dragPoint), 0.1, ORANGE);
                    DrawLine3D(toRay(bodyPoint*world.bodies[0].getTransform()), toRay(dragPoint), BLACK);
                }
            EndMode3D();
        EndDrawing();
        if (firstFrame) {
            firstFrame = false;
        }
    }
}