#include "bEngine/rigidbody.hpp"

using namespace bEngine; 
//using namespace bMath;

void rigidbody::integrate(float time) {

    bMath::float3 acceleration = forceAccum*inverseMass;
    position += linearVelocity*time+acceleration*time*time;
    linearVelocity += acceleration*time;

    bMath::Matrix3 rotmat =bMath::QuaternionToMatrix(orientation);
    bMath::Matrix3 inverseInertiaTensorWorld = bMath::transpose(rotmat)*inverseInertiaTensor*rotmat;

    bMath::float3 angularAcceleration = torqueAccum*inverseInertiaTensorWorld;
    orientation = rotate(orientation, angularVelocity*time);
    angularVelocity += angularAcceleration*time;

    clearAccumlators();
}

void rigidbody::clearAccumlators() {
    forceAccum = bMath::float3();
    torqueAccum = bMath::float3();
}

void rigidbody::addForce(const bMath::float3 &force) {
    forceAccum += force;
}

void rigidbody::addTorque(const bMath::float3 &torque) {
    torqueAccum += torque;
}

void rigidbody::addForceAtPoint(const bMath::float3 &force, const bMath::float3 &point) {
    bMath::float3 pt = point;
    pt -= position;
    forceAccum += force;
    torqueAccum += cross(pt, force);
}