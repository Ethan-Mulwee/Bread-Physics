#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

#include "contacts.hpp"
#include "rigidbody.hpp"

namespace bEngine {
  struct CollisionData {
    Contact* contacts;
    int contactsLeft;
  };

  struct Primitive {
    RigidBody* body;
    bMath::Matrix4 offset;
  };

  struct Sphere : public Primitive {
    float radius;
  };

  struct CollisionDetector {
    unsigned sphereAndSphere(const Sphere &one, const Sphere &two, CollisionData* data);
  };

}

#endif