#ifndef BENGINE_CONTACTS
#define BENGINE_CONTACTS

class ContactResolver;

class Contact {
  public:
    RigidBody* body[2];
    float friction;
    float restitution;
    bMath::float3 contactPoint;
    bMath::float3 contactNormal;
    float penetration;

  public:
    void addContacts(unsigned count) {
      contactsLeft -= count;
      contactCount += count;

      contacts += count;
    }
};

class ContactResolver {

};

#endif