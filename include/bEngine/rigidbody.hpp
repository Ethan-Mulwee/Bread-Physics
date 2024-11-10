#ifndef BENGINE_BODY
#define BENGINE_BODY

#include "bMath/bMath.hpp"

namespace bEngine {
    class rigidbody {
        protected:
            float inverseMass;
            bMath::float3 position;
            bMath::float4 orientation;
            bMath::float3 forceAcumm;
            bMath::float3 torqueAcumm;
            bMath::Matrix3 inertiaTensor;
        public:
            rigidbody() {};

            void setPosition() {

            }

            void getPosition() {
                
            }
        private:

            void integrate(float time);

            void clearAccumlators();
    };
}

#endif