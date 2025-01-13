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
    // for (int i = 0; i < 500; i++) {
    //   float max = DBL_MAX;
    //   unsigned maxIndex = contacts.count();
    //   for (int j = 0; j < contacts.count(); j++) {
    //     float sepVel = -contacts[j].getClosingVelocity(bodies);
    //     if (sepVel < max && (sepVel < 0 || contacts[j].penetration > 0)) {
    //       max = sepVel;
    //       maxIndex = j;
    //     }
    //   }
    //   if (maxIndex == contacts.count()) break;
    //   float closingVelocity = contacts[maxIndex].getClosingVelocity(bodies);
    //   if (closingVelocity < 0) continue;
    //   float separatingVelocity = -closingVelocity*(1.0f);
    //   bMath::float3 impluse(separatingVelocity*(1.0f/bodies[contacts[maxIndex].body1].inverseMass),0,0);
    //   impluse = impluse * bMath::transpose(contacts[maxIndex].getContactBasis());
    //   drawVector(impluse, ORANGE);
    //   bMath::float3 bodyPoint = (contacts[maxIndex].contactPoint - bodies[contacts[maxIndex].body1].position);
    //   bMath::float3 implusiveTorque = bMath::cross(impluse, bodyPoint);
    //   drawVector(implusiveTorque, PURPLE);
    //   bMath::float3 bodyVector = bodies[contacts[maxIndex].body1].position - contacts[maxIndex].contactPoint;
    //   bMath::matrix3 orientationMatrix = bMath::quaternionToMatrix(bodies[contacts[maxIndex].body1].orientation);
    //   bMath::matrix3 inverseInertiaTensorWorld = bMath::transpose(orientationMatrix)*bodies[contacts[maxIndex].body1].inverseInertiaTensor*orientationMatrix;

    //   float linearFactor = dot(bMath::normalized(bodyVector), bMath::normalized(impluse));

    //   bodies[contacts[maxIndex].body1].linearVelocity += impluse*bodies[contacts[maxIndex].body1].inverseMass*linearFactor;
    //   bodies[contacts[maxIndex].body1].angularVelocity += implusiveTorque*(1-linearFactor)*inverseInertiaTensorWorld*-1; /* += bMath::float3(-1,0,0); */
    // }
}

void World::resolveContacts(float time) {
    adjustPositions(time, 1);
    adjustVelocities(time, 1);
    contacts.reset();
}