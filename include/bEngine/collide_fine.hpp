#ifndef BENGINE_COLLIDE_FINE
#define BENGINE_COLLIDE_FINE

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
}

#endif