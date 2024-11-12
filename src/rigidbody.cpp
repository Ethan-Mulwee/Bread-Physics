#include "bEngine/rigidbody.hpp"

using namespace bEngine; 
using namespace bMath;

void rigidbody::integrate(float time) {

    float3 acceleration = forceAcumm*inverseMass;
    position += linearVelocity*time+acceleration*time*time;
    linearVelocity += acceleration*time;

    bMath::Matrix3 rotmat =bMath::QuaternionToMatrix(orientation);
    bMath::Matrix3 inverseInertiaTensorWorld = bMath::transpose(rotmat)*inverseInertiaTensor*rotmat;

    float3 angularAcceleration = torqueAcumm*inverseInertiaTensorWorld;
    orientation = rotate(orientation, angularAcceleration);
    angularVelocity += angularAcceleration;

    clearAccumlators();
}

void rigidbody::clearAccumlators() {
    forceAcumm = float3();
    torqueAcumm = float3();
}

void rigidbody::addForce(const bMath::float3 &force){
    forceAcumm += force;
}