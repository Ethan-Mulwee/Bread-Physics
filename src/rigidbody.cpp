#include "bphysics/rigidbody.hpp"

#include <iostream>

using namespace bphys; 

void RigidBody::integrate(float time) {

  using namespace smath;

  vector3 linearAcceleration = forceAccum*inverseMass;
  position += linearVelocity*time+linearAcceleration*time*time;
  linearVelocity += linearAcceleration*time;

  matrix3x3 orientationMatrix = matrix3x3_from_quaternion(orientation);
  matrix3x3 inverseInertiaTensorWorld = transpose(orientationMatrix)*inverseInertiaTensor*orientationMatrix;

  vector3 angularAcceleration = matrix3x3_transform_vector3(inverseInertiaTensorWorld, torqueAccum);
  orientation = quaternion_add_vector(orientation, (angularVelocity*time));
  angularVelocity += angularAcceleration*time;

  clearAccumlators();
}

void RigidBody::clearAccumlators() {
  forceAccum = smath::vector3{0,0,0};
  torqueAccum = smath::vector3{0,0,0};
}

void RigidBody::addForce(const smath::vector3 &force) {
  forceAccum += force;
}

void RigidBody::addTorque(const smath::vector3 &torque) {
  torqueAccum += torque;
}

void RigidBody::addForceAtPoint(const smath::vector3 &force, const smath::vector3 &point) {
  smath::vector3 pt = point;
  pt -= position;
  forceAccum += force;
  torqueAccum += cross(pt, force);
}

void RigidBody::addForceAtBodyPoint(const smath::vector3 &force, const smath::vector3 &point) {
  forceAccum += force;
  torqueAccum += cross(point, force);
}