#ifndef BENGINE_WORLD
#define BENGINE_WORLD

#include <vector>
#include "rigidbody.hpp"
#include "contacts.hpp"
#include "collide_narrow.hpp"

namespace bEngine {
  class World {
    public:
      std::vector<RigidBody*> bodies;
      std::vector<Primitive*> colliders;

    private:
      unsigned iterations = 500;

      ContactPool contacts;

      void generateContacts();

      void adjustPositions(float time, unsigned iterations);

      void adjustVelocities(float time, unsigned iterations);

      void resolveContacts(float time);

    public:
      World();

      void step(float time);
  };
}

#endif