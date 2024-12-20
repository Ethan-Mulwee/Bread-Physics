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
      bMath::float3 contactPoint;
      bMath::float3 contactNormal;
      float penetration;

    public:

  };

  class ContactResolver {

  };
}

#endif