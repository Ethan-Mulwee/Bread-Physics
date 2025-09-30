#ifndef BENGINE_FORCEGEN
#define BENGINE_FORCEGEN

#include "rigidbody.hpp"

namespace bEngine {
  class ForceGenerator {
    public:
      virtual void updateForce(RigidBody* body, float time) = 0;
  };

  class Gravity : public ForceGenerator {
    smath::vector3 gravity;

    
  };
}

#endif