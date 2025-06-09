#include "mesh.hpp"

Mesh Mesh::GenMeshCube(bMath::float3 halfSize)
{
    using namespace bMath;

    Mesh mesh;
    mesh.vertices = {
        vector3(0,0,0),
        vector3(0,0,1),
        vector3(0,1,0),
        vector3(0,1,1),
        vector3(1,0,0),
        vector3(1,0,1),
        vector3(1,1,0),
        vector3(1,1,1)
    };

    for (int i = 0; i < 8; i++) {
        mesh.vertices[i] *= halfSize;
        mesh.vertices[i] += vector3(0.5,0.5,0.5);
    }

    mesh.indices = {
        //Top
        2, 6, 7, 2, 3, 7, 
        //Bottom
        0, 4, 5, 0, 1, 5, 
        //Left
        0, 2, 6, 0, 4, 6, 
        //Right
        1, 3, 7, 1, 5, 7, 
        //Front
        0, 2, 3, 0, 1, 3, 
        //Back
        4, 6, 7, 4, 5, 7 
    };
}