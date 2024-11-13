#ifndef BENGINE_BODY
#define BENGINE_BODY

#include "bMath/bMath.hpp"

namespace bEngine {
    class RigidBody {
        public:
            float inverseMass;
            bMath::float3x3 inverseInertiaTensor;

            bMath::float3 position;
            bMath::float4 orientation = bMath::float4(1,0,0,0);

            bMath::float3 linearVelocity;
            bMath::float3 angularVelocity;

            bMath::float3 forceAccum;
            bMath::float3 torqueAccum;
            

        public:
            RigidBody() {};

            bMath::Matrix4 getTransform() {
                bMath::float3x3 o = QuaternionToMatrix(orientation);
                return bMath::Matrix4(
                    o(0,0), o(0,1), o(0,2), position.x,
                    o(1,0), o(1,1), o(1,2), position.y,
                    o(2,0), o(2,1), o(2,2), position.z,
                    0,      0,      0,      1
                );
            }

            void addForce(const bMath::float3 &force);

            void addTorque(const bMath::float3 &torque);

            void addForceAtPoint(const bMath::float3 &force, const bMath::float3 &point);

            void addForceAtBodyPoint(const bMath::float3 &force, const bMath::float3 &point);

            void integrate(float time);
            
        private:


            void clearAccumlators();
    };
}

#endif