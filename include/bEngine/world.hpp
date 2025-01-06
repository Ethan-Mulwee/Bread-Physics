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
      std::vector<Primitive> colliders;

    public:
      unsigned iterations = 500;

      ContactPool contacts;

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