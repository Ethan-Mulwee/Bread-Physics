#ifndef BENGINE_BODY
#define BENGINE_BODY

#include "mesh.hpp"

namespace bEngine {
    class body {
        public:
        float inverseMass;
        bMath::float3 position;
        bMath::float4 orientation;
        bMath::float3 forceAcumm;
        bMath::float3 torqueAcumm;
        bMath::Matrix3 inertiaTensor;

        void integrate(float time);

        void clearAccumlators();
    };
}

#endif