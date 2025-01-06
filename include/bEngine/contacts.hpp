#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include <bMath/bMath.hpp>

#include "rigidbody.hpp"

namespace bEngine {
  class Contact {
    public:
      size_t body1 = -1;
      size_t body2 = -1;
      float friction;
      float restitution;
      bMath::float3 contactPoint;
      bMath::float3 contactNormal;
      float penetration;
  };
}

#endif