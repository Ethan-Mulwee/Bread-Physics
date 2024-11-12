#ifndef BENGINE_WORLD
#define BENGINE_WORLD

#include <vector>
#include "rigidbody.hpp"

namespace bEngine {
  class World {
    public:
    std::vector<RigidBody*> bodies;

    void step(float time);
  };
}

#endif