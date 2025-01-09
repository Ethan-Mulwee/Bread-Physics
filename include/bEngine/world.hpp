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
      unsigned iterations = 500;

      ContactPool contacts;

      void generateContacts();

      void prepareContacts(float time);

      void adjustPositions(float time);

      void adjustVelocities(float time);

      void resolveContacts(/* unsigned numContacts, use contactCount*/ float time);

    public:
      World();

      void step(float time);
  };
}

#endif