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
        public:
            World();
            void step(float time);
        private:
            void generateContacts();

            void resolveContacts(float time);
            void adjustPositions(float time, unsigned iterations);
            void adjustVelocities(float time, unsigned iterations);
    };
}

#endif