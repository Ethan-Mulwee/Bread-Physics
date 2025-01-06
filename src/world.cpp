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


void cubeFloor(size_t cube, const float floorHeight, World* data)
{
  using namespace bMath;
  if (data->contactsLeft <= 0) return;

  float l = data->bodies[cube].collider.width;

  float3 vertices[8] = {
    float3( l, l, l),
    float3( l, l,-l),
    float3( l,-l, l),
    float3( l,-l,-l),
    float3(-l, l, l),
    float3(-l, l,-l),
    float3(-l,-l, l),
    float3(-l,-l,-l)
  };

  for (int i = 0; i < 8; i++) {
    float3 position = vertices[i]*data->bodies[cube].getTransform();
    if (position.y < floorHeight) {
      Contact contact;
      contact.contactNormal = float3(0,1,0);
      contact.contactPoint = position;
      contact.penetration = -floorHeight+position.y;
      
      contact.body1 = cube;
      contact.friction = data->friction;
      contact.restitution = data->restitution;

      data->addContact(contact);
    }
  }
}

void bEngine::World::generateContacts() {
  for (int i = 0; i < bodies.size(); i++) {
    cubeFloor(i, -2.0f, this);
  }
}

void bEngine::World::prepareContacts(float time) {
  for (Contact* contact=contacts; contact < (contacts+contactCount); contact++) {
    // Swap so null body is always body2
    if (contact->body1 == -1) {
      contact->body1 = contact->body2;
      contact->body2 = -1;
    }

    // bMath::float3 contactTagent[2];

    // if (abs(contact->contactNormal.x) > abs(contact->contactNormal.y)) {
    //   const float s = 1.0f/contact->
    // }
  }
}

void bEngine::World::adjustPositions(float time) {

}

void bEngine::World::adjustVelocities(float time) {

}

void bEngine::World::resolveContacts(float time) {
  for (int i = 0; i < contactCount; i++) {
    bodies[contacts[i].body1].position = bodies[contacts[i].body1].position - (contacts[i].contactNormal*contacts[i].penetration);
    bodies[contacts[i].body1].addForce(contacts[i].contactNormal*restitution);
  }
  resetContacts();
}

void bEngine::World::resetContacts() {
  contactPtr = contacts;
  contactsLeft = maxContacts;
  contactCount = 0;
}