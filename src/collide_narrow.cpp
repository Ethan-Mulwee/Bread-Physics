#include "bEngine/collide_narrow.hpp"

using namespace bEngine;

// unsigned sphereSphere(const Primitive &one, const Primitive &two, CollisionData* data); {
//   using namespace bMath;
//   if (data->contactsLeft <= 0) return 0;
    
//   float3 positionOne = getColumnVector(one.transform, 3).xyz;
//   float3 positionTwo = getColumnVector(two.transform, 3).xyz;

//   float3 midline = positionOne - positionTwo;
//   float size = midline.length();

//   if (size <= 0.0f || size >= one.radius+two.radius) {
//     return 0;
//   }

//   float3 normal = midline * (1.0f/size);

//   // Contact* contact = data->contacts;
//   // contact->contactNormal = normal;
//   // contact->contactPoint = positionOne + midline * 0.5f;
//   // contact->penetration = one.radius+two.radius-size;
    
//   // contact->body[0] = one.body;
//   // contact->body[1] = two.body;
//   // contact->friction = data->friction;
//   // contact->restitution = data->restitution;

//   // data->addContacts(1);
//   // return 1;
// }

void CollisionDetector::cubeCube(const Primitive &one, const Primitive &two, ContactPool &contacts) {
    
    using namespace smath;
    
    float smallestPeneration = DBL_MAX;
    int smallestIndex = -1;

    vector3 toCenter = two.getAxis(3) - one.getAxis(3);

    vector3 axes[15] = {
        one.getAxis(0), one.getAxis(1), one.getAxis(2),
        two.getAxis(0), two.getAxis(1), two.getAxis(2), 
        cross(one.getAxis(0), two.getAxis(0)), cross(one.getAxis(0), two.getAxis(1)), cross(one.getAxis(0), two.getAxis(2)),
        cross(one.getAxis(1), two.getAxis(0)), cross(one.getAxis(1), two.getAxis(1)), cross(one.getAxis(1), two.getAxis(2)), 
        cross(one.getAxis(2), two.getAxis(0)), cross(one.getAxis(2), two.getAxis(1)), cross(one.getAxis(2), two.getAxis(2)),
    };

    unsigned bestSingleAxis;

    for (int i = 0; i < 15; i++) {
        if (axes[i].square_length() < 0.0001f) continue;
        axes[i].normalize();
        float peneration = penetrationOnAxis(one, two, axes[i], toCenter);
        if (peneration < smallestPeneration) {
            smallestPeneration = peneration;
            smallestIndex = i;
        }

        if (i == 5) {
            bestSingleAxis = smallestIndex;
        }
    }

    // assert(smallestIndex != -1);

    // penetration logic
    if (smallestPeneration > 0) {
        if (smallestIndex < 3) {
            // Vertex from box two on face of box one
            vector3 normal = one.getAxis(smallestIndex);
            if (dot(normal, toCenter) > 0) {
                normal = normal * -1;
            }


            vector3 vertex = two.dimensions;
            if (dot(two.getAxis(0), normal) < 0) vertex.x = -vertex.x;
            if (dot(two.getAxis(1), normal) < 0) vertex.y = -vertex.y;
            if (dot(two.getAxis(2), normal) < 0) vertex.z = -vertex.z;

            Contact contact;
            contact.contactNormal = normal;
            contact.penetration = smallestPeneration;
            contact.contactPoint = matrix4x4_transform_vector3(two.getTransform(), vertex);
            contact.body[0] = one.body;
            contact.body[1] = two.body;

            contact.debugLabel = "Face of one and vertex of two";

            contacts.push(contact);
        }
        else if (smallestIndex < 6) {
            // Vertex from box one on face of box two
            vector3 normal = two.getAxis(smallestIndex-3);
            if (dot(normal, toCenter*-1.0f) > 0) {
                normal = normal * -1;
            }

            vector3 vertex = one.dimensions;
            if (dot(one.getAxis(0), normal) < 0) vertex.x = -vertex.x;
            if (dot(one.getAxis(1), normal) < 0) vertex.y = -vertex.y;
            if (dot(one.getAxis(2), normal) < 0) vertex.z = -vertex.z;

            Contact contact;
            contact.contactNormal = normal;
            contact.penetration = smallestPeneration;
            contact.contactPoint = matrix4x4_transform_vector3(one.getTransform(), vertex);
            contact.body[0] = two.body;
            contact.body[1] = one.body;

            contact.debugLabel = "Face of two and vertex of one";

            contacts.push(contact);
        }
        else {
            // edge-edge contact
            int index = smallestIndex;
            index -= 6;

            unsigned oneAxisIndex = index / 3;
            unsigned twoAxisIndex = index % 3;
            vector3 oneAxis = one.getAxis(oneAxisIndex);
            vector3 twoAxis = two.getAxis(twoAxisIndex);
            vector3 axis = cross(oneAxis, twoAxis);
            axis.normalize();

            if (dot(axis,toCenter) < 0) axis *= -1.0f;

            vector3 pointOne = one.dimensions;
            vector3 pointTwo = two.dimensions;

            for (int i = 0; i < 3; i++) {
                if (i == oneAxisIndex) pointOne.data[i] = 0;
                else if (dot(one.getAxis(i), axis) > 0) pointOne.data[i] *= -1.0f;

                if (i == twoAxisIndex) pointTwo.data[i] = 0;
                else if (dot(two.getAxis(i), axis) < 0) pointTwo.data[i] *= -1.0f;
            }

            pointOne = matrix4x4_transform_vector3(one.getTransform(), pointOne);
            pointTwo = matrix4x4_transform_vector3(two.getTransform(), pointTwo);

            vector3 vertex;

            // // Edge edge contact vertex bullshit
            const vector3 pOne = pointOne;
            const vector3 dOne = oneAxis;
            float oneSize = one.dimensions.data[oneAxisIndex];

            const vector3 pTwo = pointTwo;
            const vector3 dTwo = twoAxis;
            float twoSize = two.dimensions.data[twoAxisIndex];

            bool useOne = (bestSingleAxis > 2);

            vector3 toSt, cOne, cTwo;
            float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
            float denom, mua, mub;

            smOne = dOne.square_length();
            smTwo = dTwo.square_length();
            dpOneTwo = dot(dTwo, dOne);

            toSt = pOne - pTwo;
            dpStaOne = dot(dOne, toSt);
            dpStaTwo = dot(dTwo, toSt);

            denom = smOne * smTwo - dpOneTwo * dpOneTwo;

            // Zero denominator indicates parrallel lines
            if (abs(denom) < 0.0001f) {
                vertex = useOne?pOne:pTwo;
            }

            mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
            mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

            // If either of the edges has the nearest point out
            // of bounds, then the edges aren't crossed, we have
            // an edge-face contact. Our point is on the edge, which
            // we know from the useOne parameter.
            if (mua > oneSize ||
                mua < -oneSize ||
                mub > twoSize ||
                mub < -twoSize)
            {
                vertex = useOne?pOne:pTwo;
            }
            else
            {
                cOne = pOne + dOne * mua;
                cTwo = pTwo + dTwo * mub;

                vertex = cOne * 0.5 + cTwo * 0.5;
            }
            

            Contact contact;
            contact.contactNormal = axis;
            contact.penetration = smallestPeneration;
            contact.contactPoint = vertex;
            contact.body[0] = two.body;
            contact.body[1] = one.body;

            contact.debugLabel = "Edge edge";

            contacts.push(contact);
        }
    }
}

void CollisionDetector::cubeFloor(const Primitive &cube, const float floorHeight, ContactPool &contacts)
{
    using namespace smath;
    if (contacts.room() <= 0) return;

    float l = cube.dimensions.x;

    vector3 vertices[8] = {
        vector3{ l, l, l},
        vector3{ l, l,-l},
        vector3{ l,-l, l},
        vector3{ l,-l,-l},
        vector3{-l, l, l},
        vector3{-l, l,-l},
        vector3{-l,-l, l},
        vector3{-l,-l,-l}
    };

    for (int i = 0; i < 8; i++) {
        vector3 position = matrix4x4_transform_vector3(cube.getTransform(), vertices[i]);
        if (position.y < floorHeight) {
            Contact contact;
            contact.contactNormal = vector3{0,1,0};
            contact.contactPoint = position;
            contact.penetration = -(position.y-floorHeight);
            
            contact.body[0] = cube.body;
            contact.friction = 0.2f;
            contact.restitution = 0.2f;

            contacts.push(contact);
        }
    }
}