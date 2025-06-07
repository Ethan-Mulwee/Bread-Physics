#ifndef BMATH_PHYSICS
#define BMATH_PHYSICS

#include "matrix.hpp"
#include "vector.hpp"

namespace bMath {
  // Following inertia tensor function's math for common shapes comes from: Game Physics Engine Development p.493-495

  // Returns the inertia tensor of a cuboid from the center of mass
  inline matrix3 InertiaTensorCuboid(float m, float dx, float dy, float dz) {
    float c = m*((float)1/12);
    return matrix3(
      c*(dy*dy+dz*dz), 0, 0,
      0, c*(dx*dx+dz*dz), 0,
      0, 0, c*(dx*dx+dy*dy)
    );
  }

  // Returns the inertia tensor of a sphere from the center of mass 
  inline matrix3 InertiaTensorSphere(float m, float r) {
    float c = ((float)2/3)*m*r*r;
    return matrix3(
      c,0,0,
      0,c,0,
      0,0,c
    );
  }

  // Returns the inertia tensor of a ellipsoid from the center of mass
  // m - mass, r - radii
  inline matrix3 InertiaTensorEllipsoid(float m, float3 r) {
    float c = ((float)1/5)*m;
    return matrix3(
      c*(r.y*r.y+r.z*r.z),0,0,
      0,c*(r.x*r.x+r.z*r.z),0,
      0,0,c*(r.x*r.x+r.y*r.y)
    );
  }

  // Returns the intertia tensor of a cylidner from the center of mass
  // m - mass, h - height, r - radius 
  inline matrix3 InertiaTensorCylinder(float m, float h, float r) {
    return matrix3(
      (1/12.0f)*h*h+(1/4.0f)*m*r*r,0,0,
      0,(1/2.0f)*m*r*r,0,
      0,0,(1/12.0f)*h*h+(1/4.0f)*m*r*r
    );
  }

  // Returns the inertia tensor of a tube from the center of mass
  // m - mass, h - height, ro - outer radius, ri inner radius
  inline matrix3 InertiaTensorTube(float m, float h, float ro, float ri) {
    return matrix3(
      (1/12.0f)*m*h*h+(1/4.0f)*m*(ro*ro+ri*ri),0,0,
      0,(1/2.0f)*m*(ro*ro+ri*ri),0,
      0,0,(1/12.0f)*m*h*h+(1/4.0f)*m*(ro*ro+ri*ri)
    );
  }

  // Returns the inertia tensor of a cone from the center of mass
  // m - mass, h - height, r - radius
  inline matrix3 InertiaTensorCone(float m, float h, float r) {
    return matrix3(
      (3/80.0f)*m*h*h+(3/20.0f)*m*r*r,0,0,
      0,(3/10.0f)*m*r*r,0,
      0,0,(3/5.0f)*m*h*h+(3/20.0f)*m*r*r
    );
  }
} // namespace bMath

#endif