#ifndef BENGINE_FORCEGEN
#define BENGINE_FORCEGEN

#include "rigidbody.hpp"

namespace bEngine {
  class ForceGenerator {
    public:
      virtual void updateForce(RigidBody* body, float time) = 0;
  };

  class Gravity : public ForceGenerator {
    bMath::float3 gravity;

    
  };
}

#endif