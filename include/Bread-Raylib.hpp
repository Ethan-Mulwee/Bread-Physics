#include <raylib.h>
#include "bEngine/body.hpp"
#include <math.h>
#include <raymath.h>

Vector3 ToRay(bMath::float3 v) {
    return Vector3{v.x,v.y,v.z};
}

void DrawMesh(bEngine::Mesh mesh) {
    for (int i = 0; i < mesh.indices.size()/3; i++) {
        // OBJ files index verts at 1
        bMath::float3 p0 = mesh.vertices[mesh.indices[i*3]-1];
        bMath::float3 p1 = mesh.vertices[mesh.indices[i*3+1]-1];
        bMath::float3 p2 = mesh.vertices[mesh.indices[i*3+2]-1];
        bMath::float3 v1 = p2-p0;
        bMath::float3 v2 = p1-p0;
        v1.normalize();
        v2.normalize();
        bMath::float3 normal = cross(v1, v2);
        normal.normalize();
        bMath::float3 light = bMath::float3(-1,-1,-1);
        light.normalize();
        float d = dot(normal,light);
        unsigned char value = Clamp(d*255, 0, 255);
        DrawTriangle3D(ToRay(p0), ToRay(p1), ToRay(p2), Color{value,value,value,255});
        // DrawLine3D(ToRay(p0), ToRay(p1), GRAY);
        // DrawLine3D(ToRay(p1), ToRay(p2), GRAY);
        // DrawLine3D(ToRay(p2), ToRay(p0), GRAY);
    }
}

