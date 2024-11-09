#include <iostream>
#include "bMath/bMath.hpp"
#include <raylib.h>
#include "bEngine/body.hpp"
#include "Bread-Raylib.hpp"

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

    bMath::Triangle triangle(bMath::float3(1,-1,0), bMath::float3(1,2,2), bMath::float3(4,0,0));
    bMath::Ray ray(bMath::float3(-1,-2,2), bMath::float3(1,1,-0.5));

    bMath::RayIntersection intersection = Raycast(ray, triangle);
    std::cout << intersection.point << std::endl;

    bMath::Matrix<float,3,3> m;
    std::cout << m << std::endl;

    bEngine::rigidbody body;

    body.mesh = bEngine::ParseMeshFromFile("Cube.obj");


    while(!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_PERSPECTIVE);
        BeginDrawing();
            ClearBackground(Color{35,35,35,255});
            BeginMode3D(camera);
                DrawGrid(5,1);
                DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
                DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength+0.5,0}, GREEN);
                DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);

                DrawTriangle3D(btr(triangle.a),btr(triangle.b),btr(triangle.c), WHITE);
                DrawRay(Ray{btr(ray.p),btr(ray.d)}, BLACK);
                DrawMesh(body.mesh);
                DrawSphere(btr(intersection.point), 0.1, RED);
            EndMode3D();
        EndDrawing();
    }
}