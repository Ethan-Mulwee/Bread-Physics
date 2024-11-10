#include <iostream>
#include "bMath/bMath.hpp"
#include <raylib.h>
#include "bEngine/rigidbody.hpp"

Camera camera;
const int axisLength = 4;

Vector3 btr(const bMath::float3 &v) {
    return Vector3{v.x,v.y,v.z};
}

Matrix btr(const bMath::Matrix4 &m) {
    return Matrix{
        m(0,0), m(0,1), m(0,2), m(0,3),
        m(1,0), m(1,1), m(1,2), m(1,3),
        m(2,0), m(2,1), m(2,2), m(2,3),
        m(3,0), m(3,1), m(3,2), m(3,3)
    };
}

bMath::float3 rtb(const Vector3 &v) {
    return bMath::float3(v.x,v.y,v.z);
}

bMath::Matrix4 rtb(const Matrix &m) {
    return bMath::Matrix4(
        m.m0, m.m4, m.m8, m.m12,
        m.m1, m.m5, m.m9, m.m13,
        m.m2, m.m6, m.m10, m.m14,
        m.m3, m.m7, m.m11, m.m15
    );
}

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