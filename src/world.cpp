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
        for (int j = 0; j < colliders.size(); j++) {
            if (j == i) continue;
            CollisionDetector::cubeCube(colliders[i], colliders[j], contacts);
        }
    }
}

// TODO: check for NaNs in this phase

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

        PenetrationResolutionResult result = contacts[maxIndex].resolvePenetration();

        float3 deltaPosition;

        // Update penetrations contacts related to the moved bodies
        for (unsigned j = 0; j < contacts.count(); j++) {
            for (unsigned b = 0; b < 2; b++) if (contacts[j].body[b]) {
                for (unsigned d = 0; d < 2; d++) {
                    if (contacts[j].body[b] == contacts[maxIndex].body[d]) {
                        deltaPosition = result.linearChange[d] + cross(result.angularChange[d], contacts[j].getContactPointRelativeToBody(b));
                        contacts[j].penetration = dot(deltaPosition, contacts[j].contactNormal)* (b ? 1 : -1);
                    }
                }
            }
        }
    }
}

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

    adjustPositions(time, 100);
    adjustVelocities(time, 100);
    contacts.reset();
}