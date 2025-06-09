#ifndef BENGINE_MESH
#define BENGINE_MESH

#include "bMath.hpp"
#include <array>

class Mesh {
    public:

        static Mesh GenMeshCube(bMath::float3 halfSize);

        std::vector<bMath::vector3> vertices;
        std::vector<int> indices;
};



#endif