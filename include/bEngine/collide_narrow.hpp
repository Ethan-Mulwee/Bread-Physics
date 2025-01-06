#ifndef BENGINE_COLLIDE_NARROW
#define BENGINE_COLLIDE_NARROW

// #include "contacts.hpp"
#include <bMath/bMath.hpp>

namespace bEngine {
  enum PrimitiveType {
    Sphere,
    Cube,
    Plane
  };

  struct Primitive {
    PrimitiveType type;
    bMath::matrix4 offset;
    float height;
    float width;
  };

  // struct CollisionDetector {
  //   unsigned sphereSphere(const Primitive &one, const Primitive &two, world* data);

  //   unsigned cubeFloor(const Primitive &cube, const float floorHeight, world* data);
  // };

}

#endif