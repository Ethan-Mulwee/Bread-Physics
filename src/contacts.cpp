#include "bEngine/contacts.hpp"

using namespace bEngine;

bMath::matrix3 Contact::getContactBasis() {
  // contactNormal = x axis
  // the rest of the basis will be calculated starting with a guess of world space Y
  // If the contact normal closer to being aligned to the world space y than the world space x then we start with the guess of world space x
  using namespace bMath;

  float3 tagentY;
  float3 tagentZ;

  if (abs(contactNormal.y) < abs(contactNormal.z)) {
    tagentZ = float3(-contactNormal.z, 0, contactNormal.x);
    tagentY = cross(contactNormal, tagentZ);
  }
  else {
    tagentY = float3(-contactNormal.y, contactNormal.x, 0);
    tagentZ = cross(contactNormal, tagentY);
  }

  return bMath::matrix3(
    contactNormal.x, tagentY.x, tagentZ.x,
    contactNormal.y, tagentY.y, tagentZ.y,
    contactNormal.z, tagentY.z, tagentZ.z
  );
}
