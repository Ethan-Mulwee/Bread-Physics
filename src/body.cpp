#include "bEngine/body.hpp"

using namespace bEngine; 
using namespace bMath;

void body::integrate(float time) {
    float3 acceleration = forceAcumm*time;
    float3 rotation = torqueAcumm*time;

    position += acceleration*time;
}

void body::clearAccumlators() {
    forceAcumm = float3();
    torqueAcumm = float3();
}