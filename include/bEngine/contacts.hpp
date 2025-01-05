#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include <bMath/bMath.hpp>

#include "rigidbody.hpp"

namespace bEngine {
  class ContactResolver;

  class Contact {
    public:
      size_t b1;
      size_t b2;
      float friction;
      float restitution;
      bMath::float3 contactPoint;
      bMath::float3 contactNormal;
      float penetration;

    public:

  };

  class ContactResolver {
    private:
      unsigned iterations;
    public:
      ContactResolver(int iterations);
      void resolveContacts(Contact *contactArray, unsigned numContacts, float time);
  };
}

#endif