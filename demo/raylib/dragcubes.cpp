#include <iostream>
#include <string>
#include <bMath/bMath.hpp>
#include <bMath/ext/raylib.hpp>
#include <bMath/ext/iostream.hpp>
#include <bMath/ext/string.hpp>
#include "bEngine/world.hpp"
#include "render.hpp"

Camera camera;
const int axisLength = 4;

int main() {
    //Raylib stuff
    InitWindow(1000,1000,"test");
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
    // body->angularVelocity = bMath::float3(1,.2,0);
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
    body2->position = bMath::float3(3,2,-1);
    body2->orientation = bMath::quaternion(0.951,0.189,0.198,-0.146);
    body2->angularVelocity = bMath::float3(1,.2,0);

    bEngine::Primitive* collider2 = new bEngine::Primitive();
    collider2->type = bEngine::PrimitiveType::Cube;
    collider2->height = 0.5f;
    collider2->width = 0.5f;
    collider2->offset = bMath::matrix4::identity();
    collider2->body = body2;

    world.bodies.push_back(body2);
    world.colliders.push_back(collider2);
    ///////////////////////////////////////////////////////////////////////////////////////

    bMath::float3 bodyPoint;
    bMath::float3 dragPoint;
    bool dragging = false;

    bMath::float2 mouseDeltaPos;

    float cameraAngle = M_PI/4;
    float zoom = 10.0f;

    while(!WindowShouldClose()) {

        mouseDeltaPos = ConvertBread(GetMouseDelta());
        zoom += GetMouseWheelMove()*-1.0f;
        camera.position = ConvertRay((bMath::float3(std::cos(cameraAngle),0,std::sin(cameraAngle))*zoom)+bMath::float3(0,camera.position.y,0));

        Ray screenRay = GetScreenToWorldRay(GetMousePosition(), camera);
        RayCollision collision = GetRayCollisionMesh(screenRay, renderer.cubeModel.meshes[0], ConvertRay(world.bodies[0]->getTransform()));

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_CONTROL)) {
            if (collision.hit) {
                bodyPoint = world.bodies[0]->positionToBodySpace((ConvertBread(collision.point)));
                dragPoint = ConvertBread(collision.point);
                dragging = true;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            camera.position = ConvertRay(ConvertBread(camera.position)+(bMath::float3(0, mouseDeltaPos.y, 0)*0.02f));
            cameraAngle += mouseDeltaPos.x*0.003f;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && !dragging && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (collision.hit) {
                bodyPoint = world.bodies[0]->positionToBodySpace((ConvertBread(collision.point)));
                bMath::float3 worldSpaceBodyPoint = bodyPoint*world.bodies[0]->getTransform();
                world.bodies[0]->addForceAtPoint(ConvertBread(screenRay.direction)*3000, worldSpaceBodyPoint);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
        }


        if (dragging) {
            bMath::float3 viewVector = bMath::normalized(ConvertBread(camera.target) - ConvertBread(camera.position));
            bMath::float3 tagentViewVector = bMath::normalized(bMath::cross(ConvertBread(camera.up), viewVector));
            bMath::float3 cameraUpVector = bMath::cross(viewVector, tagentViewVector);
            bMath::matrix3 spaceRotation(
              tagentViewVector.x, cameraUpVector.x, viewVector.x,
              tagentViewVector.y, cameraUpVector.y, viewVector.y,
              tagentViewVector.z, cameraUpVector.z, viewVector.z
            );
            dragPoint += bMath::float3(-mouseDeltaPos.x*0.005, -mouseDeltaPos.y*0.005, 0)*spaceRotation;
            bMath::float3 worldSpaceBodyPoint = bodyPoint*world.bodies[0]->getTransform();
            bMath::float3 force = dragPoint - worldSpaceBodyPoint;
            world.bodies[0]->addForceAtPoint(force*15, worldSpaceBodyPoint);
        }

        for (int i = 0; i < world.bodies.size(); i++) {
          world.bodies[i]->addForce(bMath::float3(0,-9.8,0)*(1.0f/world.bodies[i]->inverseMass));
        }


        // UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            std::string angularVelocityStr = "Angular Velocity: ";
            angularVelocityStr += toString(world.bodies[0]->angularVelocity);
            DrawText(angularVelocityStr.c_str(), 10, 10, 20, WHITE);

            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                if (dragging) {
                    DrawSphere(ConvertRay(bodyPoint*world.bodies[0]->getTransform()), 0.1,RED);
                    DrawSphere(ConvertRay(dragPoint), 0.1, ORANGE);
                    DrawLine3D(ConvertRay(bodyPoint*world.bodies[0]->getTransform()), ConvertRay(dragPoint), BLACK);
                }



                world.step(1/200.0f);
                renderer.render(world);
            EndMode3D();
        EndDrawing();
    }
}