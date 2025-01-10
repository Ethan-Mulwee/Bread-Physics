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

void bEngine::World::adjustPositions(float time) {

}

void bEngine::World::adjustVelocities(float time) {

}

#include <iostream>

void bEngine::World::resolveContacts(float time) {
  for (int i = 0; i < contacts.count(); i++) {
    std::cout << contacts[i].getClosingVelocity(bodies) << "\n";
    bodies[contacts[i].body1].position = bodies[contacts[i].body1].position - (contacts[i].contactNormal*contacts[i].penetration);
    bodies[contacts[i].body1].addForce(contacts[i].contactNormal*0.2f);
  }
  contacts.reset();
}