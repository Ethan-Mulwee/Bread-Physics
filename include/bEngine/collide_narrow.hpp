#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

#include "contacts.hpp"
#include "rigidbody.hpp"

namespace bEngine {
  struct CollisionData {
    Contact* contacts;
    int contactsLeft;
    unsigned contactCount;

    float friction;
    float restitution;

    void addContacts(unsigned count) {
      contactsLeft -= count;
      contactCount += count;

      contacts += count;
    }
  };

  struct Primitive {
    RigidBody* body;
    bMath::matrix4 offset;
    bMath::matrix4 transform;
  };

  struct Sphere : public Primitive {
    float radius;
  };

  struct CollisionDetector {
    unsigned sphereAndSphere(const Sphere &one, const Sphere &two, CollisionData* data);
  };

}

#endif