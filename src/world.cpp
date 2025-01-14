#include "bEngine/world.hpp"

using namespace bEngine;

World::World() {

}

void World::step(float time){
    generateContacts();
    resolveContacts(time);
    for (int i = 0; i < bodies.size(); i++) {
        bodies[i]->integrate(time);
    }
}




void World::generateContacts() {
    for (int i = 0; i < colliders.size(); i++) {
        CollisionDetector::cubeFloor(colliders[i], 0.0f, contacts);
    }
}

void World::adjustPositions(float time, unsigned iterations) {
    using namespace bMath;

    for (int i = 0; i < iterations; i++) {

        float maxPenetration = 0;
        unsigned maxIndex = contacts.count();

        for (int j = 0; j < contacts.count(); j++) {
            float penetration = contacts[j].penetration;
            if (penetration > maxPenetration) {
                maxPenetration = penetration;
                maxIndex = j;
            }
        }

        if (maxIndex == contacts.count()) break;

        contacts[maxIndex].resolvePenetration();
    }
}

#include <bMath/ext/iostream.hpp>

void World::adjustVelocities(float time, unsigned iterations) {
    using namespace bMath;

    for (int i = 0; i < iterations; i++) {

        float maxClosingVelocity = 0;
        unsigned maxIndex = contacts.count();

        for (int j = 0; j < contacts.count(); j++) {
            float closingVelocity = contacts[j].getClosingVelocity();
            if (closingVelocity > maxClosingVelocity) {
                maxClosingVelocity = closingVelocity;
                maxIndex = j;
            }
        }

        if (maxIndex == contacts.count()) break;

        contacts[maxIndex].resolveVelocity();
    }
}

void World::resolveContacts(float time) {
    adjustPositions(time, 1);
    adjustVelocities(time, 1);
    contacts.reset();
}