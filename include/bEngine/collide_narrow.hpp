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

  struct Cube : public Primitive {
    float halfSize;
  };

  struct CollisionDetector {
    unsigned sphereSphere(const Sphere &one, const Sphere &two, CollisionData* data);

    unsigned cubeFloor(const Cube &cube, const float floorHeight, CollisionData* data);
  };

}

#endif