#include "bEngine/rigidbody.hpp"

using namespace bEngine; 
//using namespace bMath;

void RigidBody::integrate(float time) {

  bMath::float3 acceleration = forceAccum*inverseMass;
  position += linearVelocity*time+acceleration*time*time;
  linearVelocity += acceleration*time;

  bMath::matrix3 orientationMatrix = bMath::quaternionToMatrix(orientation);
  bMath::matrix3 inverseInertiaTensorWorld = bMath::transpose(orientationMatrix)*inverseInertiaTensor*orientationMatrix;

  bMath::float3 angularAcceleration = torqueAccum*inverseInertiaTensorWorld;
  orientation = orientation*bMath::VectortoQuaternion(angularVelocity*time);
  angularVelocity += angularAcceleration*time;

  clearAccumlators();
}

void RigidBody::clearAccumlators() {
  forceAccum = bMath::float3();
  torqueAccum = bMath::float3();
}

void RigidBody::addForce(const bMath::float3 &force) {
  forceAccum += force;
}

void RigidBody::addTorque(const bMath::float3 &torque) {
  torqueAccum += torque;
}

void RigidBody::addForceAtPoint(const bMath::float3 &force, const bMath::float3 &point) {
  bMath::float3 pt = point;
  pt -= position;
  forceAccum += force;
  torqueAccum += cross(pt, force);
}

void RigidBody::addForceAtBodyPoint(const bMath::float3 &force, const bMath::float3 &point) {
  forceAccum += force;
  torqueAccum += cross(point, force);
}