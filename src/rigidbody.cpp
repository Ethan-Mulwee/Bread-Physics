#include "bEngine/rigidbody.hpp"

using namespace bEngine; 
//using namespace bMath;

void RigidBody::integrate(float time) {

  bm::float3 acceleration = forceAccum*inverseMass;
  position += linearVelocity*time+acceleration*time*time;
  linearVelocity += acceleration*time;

  bm::matrix3 rotmat = bm::quaternionToMatrix(orientation);
  bm::matrix3 inverseInertiaTensorWorld = bm::transpose(rotmat)*inverseInertiaTensor*rotmat;

  bm::float3 angularAcceleration = torqueAccum*inverseInertiaTensorWorld;
  orientation = rotate(orientation, angularVelocity*time);
  angularVelocity += angularAcceleration*time;

  clearAccumlators();
}

void RigidBody::clearAccumlators() {
  forceAccum = bm::float3();
  torqueAccum = bm::float3();
}

void RigidBody::addForce(const bm::float3 &force) {
  forceAccum += force;
}

void RigidBody::addTorque(const bm::float3 &torque) {
  torqueAccum += torque;
}

void RigidBody::addForceAtPoint(const bm::float3 &force, const bm::float3 &point) {
  bm::float3 pt = point;
  pt -= position;
  forceAccum += force;
  torqueAccum += cross(pt, force);
}

void RigidBody::addForceAtBodyPoint(const bm::float3 &force, const bm::float3 &point) {
  forceAccum += force;
  torqueAccum += cross(point, force);
}