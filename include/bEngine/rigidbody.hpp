#ifndef BENGINE_BODY
#define BENGINE_BODY

#include "bMath/bMath.hpp"

namespace bEngine {
    class rigidbody {
        public:
            float inverseMass;
            bMath::float3 position;
            bMath::float4 orientation = bMath::float4(1,0,0,0);
            bMath::float3 forceAcumm;
            bMath::float3 torqueAcumm;
            bMath::Matrix3 inertiaTensor;

        public:
            rigidbody() {};

            void setPosition() {

            }

            void getPosition() {

            }

            bMath::Matrix4 getTransform() {
                bMath::Matrix3 o = QuaternionToMatrix(orientation);
                return bMath::Matrix4(
                    o(0,0), o(0,1), o(0,2), position.x,
                    o(1,0), o(1,1), o(1,2), position.y,
                    o(2,0), o(2,1), o(2,2), position.z,
                    0,      0,      0,      1
                );
            }

            
        private:

            void integrate(float time);

            void clearAccumlators();
    };
}

#endif