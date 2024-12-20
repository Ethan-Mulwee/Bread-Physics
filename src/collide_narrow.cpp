#include "bEngine/collide_narrow.hpp"

using namespace bEngine;

unsigned CollisionDetector::sphereAndSphere(const Sphere &one, const Sphere &two, CollisionData *data) {
  using namespace bm;
  if (data->contactsLeft <= 0) return 0;
  
  float3 positionOne = getColumnVector(one.transform, 3).xyz;
  float3 positionTwo = getColumnVector(two.transform, 3).xyz;

  float3 midline = positionOne - positionTwo;
  float size = midline.length();

  if (size <= 0.0f || size >= one.radius+two.radius) {
    return 0;
  }

  float3 normal = midline * (1.0f/size);

  Contact* contact = data->contacts;
  contact->contactNormal = normal;
  contact->contactPoint = positionOne + midline * 0.5f;
  contact->penetration = one.radius+two.radius-size;
  
  contact->body[0] = one.body;
  contact->body[1] = two.body;
  contact->friction = data->friction;
  contact->restitution = data->restitution;

  data->addContacts(1);
  return 1;
}
