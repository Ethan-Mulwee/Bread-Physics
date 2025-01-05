#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

// #include "contacts.hpp"
#include <bMath/bMath.hpp>

namespace bEngine {
  struct CollisionData {
    // Contact* contacts;
    int contactsLeft;
    unsigned contactCount;

    float friction;
    float restitution;

    void addContacts(unsigned count) {
      contactsLeft -= count;
      contactCount += count;

      // contacts += count;
    }
  };

  enum PrimitiveType {
    Sphere,
    Cube
  };

  struct Primitive {
    PrimitiveType type;
    bMath::matrix4 offset;
    float height;
    float width;
  };

  struct CollisionDetector {
    unsigned sphereSphere(const Primitive &one, const Primitive &two, CollisionData* data);

    unsigned cubeFloor(const Primitive &cube, const float floorHeight, CollisionData* data);
  };

}

#endif