#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include <bMath/bMath.hpp>

#include "rigidbody.hpp"

namespace bEngine {
  class ContactResolver;

  class Contact {
    public:
      RigidBody* body[2];
      float friction;
      float restitution;
      bm::float3 contactPoint;
      bm::float3 contactNormal;
      float penetration;

    public:

  };

  class ContactResolver {

  };
}

#endif