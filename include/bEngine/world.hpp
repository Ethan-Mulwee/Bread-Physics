#ifndef BENGINE_WORLD
#define BENGINE_WORLD

#include <vector>
#include "rigidbody.hpp"
#include "contacts.hpp"
#include "collide_narrow.hpp"

namespace bEngine {
  class World {
    public:
      World();
      std::vector<RigidBody*> bodies;
      std::vector<Primitive*> colliders;

    public:
      const static unsigned maxContacts = 256;
      Contact contacts[maxContacts];
      CollisionData collisionData;
      ContactResolver resolver = ContactResolver(500);

    
    public:
      // generates contacts from contact generators and returned the number used 
      // which is needed for resovling the contacts
      unsigned generateContacts();

      void step(float time);
  };
}

#endif