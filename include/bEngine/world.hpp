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

    private:
      const static unsigned maxContacts = 256;
      Contact contacts[maxContacts];
      Contact* contactPtr = contacts;
      unsigned iterations = 500;
      int contactsLeft;
      unsigned contactCount;

      float friction;
      float restitution;

      void addContact(Contact contact) {
        contactsLeft--;
        contactCount++;

        *contactPtr = contact;
  
        contactPtr++;
      }

      void resolveContacts(/* unsigned numContacts, use contactCount*/ float time);

    public:
      World();

      void step(float time);

    private:
      void generateContacts();
  };
}

#endif