#include "bEngine/rigidbody.hpp"

using namespace bEngine; 
using namespace bMath;

void rigidbody::integrate(float time) {
    float3 acceleration = forceAcumm*time;
    float3 rotation = torqueAcumm*time;

    position += acceleration*time;
}

void rigidbody::clearAccumlators() {
    forceAcumm = float3();
    torqueAcumm = float3();
}