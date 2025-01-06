#include "bEngine/world.hpp"

using namespace bEngine;

World::World() {

}

void World::step(float time){
  generateContacts();
  resolveContacts(time);
  for (int i = 0; i < bodies.size(); i++) {
    bodies[i].integrate(time);
  }
}




void bEngine::World::generateContacts() {
  for (int i = 0; i < bodies.size(); i++) {
    CollisionDetector::cubeFloor(i, -2.0f, contacts, bodies);
  }
}

// void bEngine::World::prepareContacts(float time) {
//   for (Contact* contact=contacts; contact < (contacts+contactCount); contact++) {
//     // Swap so null body is always body2
//     if (contact->body1 == -1) {
//       contact->body1 = contact->body2;
//       contact->body2 = -1;
//     }

//     // bMath::float3 contactTagent[2];

//     // if (abs(contact->contactNormal.x) > abs(contact->contactNormal.y)) {
//     //   const float s = 1.0f/contact->
//     // }
//   }
// }

void bEngine::World::adjustPositions(float time) {

}

void bEngine::World::adjustVelocities(float time) {

}

void bEngine::World::resolveContacts(float time) {
  for (int i = 0; i < contacts.count(); i++) {
    bodies[contacts[i].body1].position = bodies[contacts[i].body1].position - (contacts[i].contactNormal*contacts[i].penetration);
    bodies[contacts[i].body1].addForce(contacts[i].contactNormal*0.2f);
  }
  contacts.reset();
}