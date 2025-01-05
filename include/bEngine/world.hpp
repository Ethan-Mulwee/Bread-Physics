#ifndef BENGINE_WORLD
#define BENGINE_WORLD

#include <vector>
#include "rigidbody.hpp"
// #include "contacts.hpp"
// #include "collide_narrow.hpp"

namespace bEngine {
  class World {
    public:
      World();
      std::vector<RigidBody> bodies;

    private:
      const static unsigned maxContacts = 256;
      // Contact contacts[maxContacts];
      // CollisionData collisionData;
      // ContactResolver resolver = ContactResolver(500);

    
    public:
      void step(float time);
  };
}

#endif