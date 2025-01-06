#ifndef BENGINE_WORLD
#define BENGINE_WORLD

#include <vector>
#include "rigidbody.hpp"
#include "contacts.hpp"
#include "collide_narrow.hpp"

namespace bEngine {
  class World {
    public:
      std::vector<RigidBody> bodies;

    public:
      const static unsigned maxContacts = 256;
      Contact contacts[maxContacts];
      Contact* contactPtr = contacts;
      unsigned iterations = 500;
      int contactsLeft = maxContacts;
      unsigned contactCount;

      float friction;
      float restitution;

      void addContact(Contact contact) {
        contactsLeft--;
        contactCount++;

        *contactPtr = contact;
  
        contactPtr++;
      }

      void resetContacts();

      void prepareContacts(float time);

      void adjustPositions(float time);

      void adjustVelocities(float time);

      void resolveContacts(/* unsigned numContacts, use contactCount*/ float time);

    public:
      World();

      void step(float time);

    private:
      void generateContacts();
  };
}

#endif