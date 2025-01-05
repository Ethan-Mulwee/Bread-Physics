#include "bEngine/world.hpp"

using namespace bEngine;

World::World() {
  // collisionData.contacts = contacts;
}

void World::step(float time){

  for (int i = 0; i < bodies.size(); i++) {
    bodies[i].integrate(time);
  }
}