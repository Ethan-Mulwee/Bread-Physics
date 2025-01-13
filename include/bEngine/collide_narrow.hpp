#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

#include "contacts.hpp"
#include "rigidbody.hpp"
#include <bMath/bMath.hpp>
#include <vector>

namespace bEngine {
  struct CollisionDetector {
    static void cubeFloor(const Primitive* cube, const float floorHeight, ContactPool &contacts);
  };
}

#endif