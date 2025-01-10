#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

#include <bMath/bMath.hpp>

#include "rigidbody.hpp"

namespace bEngine {
  struct Contact {
    size_t body1 = -1;
    size_t body2 = -1;
    float friction;
    float restitution;
    bMath::float3 contactPoint;
    bMath::float3 contactNormal;
    float penetration;

    bMath::matrix3 getContactBasis();

    float getClosingVelocity(const std::vector<RigidBody> &bodies); 
  };

  class ContactPool {
    private:
      const static unsigned maxContacts = 256;
      Contact contacts[256];
      Contact* contactPtr = contacts;
      int contactsLeft = maxContacts;
      unsigned contactCount = 0;
  
      float friction = 0.2f;
      float restitution = 0.2f;

    public:
      void push(Contact contact) {
        contactsLeft--;
        contactCount++;
  
        *contactPtr = contact;
  
        contactPtr++;
      }

      unsigned room() {
        return contactsLeft;
      }

      unsigned count() {
        return contactCount;
      }

      unsigned capacity() {
        return maxContacts;
      }

      // Contact operator[](const int i) const { return contacts[i]; }
      Contact& operator[](const int i) { return contacts[i]; }

      void reset() {
        contactPtr = contacts;
        contactsLeft = maxContacts;
        contactCount = 0;
      }
  };
}

#endif