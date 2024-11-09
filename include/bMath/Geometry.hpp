#ifndef BMATH_GEOMETRY
#define BMATH_GEOMETRY

#include "Vector.hpp"
#include "Matrix.hpp"
#include <vector>

namespace bMath {
    // Returns vector transformed by matrix (will shrink dimension of vector of transformation does so)
    template <typename T, int n, int cols>
    Vector<T, cols> Transform(const Vector<T,n> &v, const Matrix<T,n,cols> &m) {

    }

    struct Triangle {
        float3 a,b,c;

        Triangle(float3 a, float3 b, float3 c)
        : a(a), b(b), c(c) {}

        float3 getNormal() const {
            float3 crossProduct = cross(b-a,c-a);
            crossProduct.normalize();
            return crossProduct;
        }
    };

    struct RayIntersection {
        unsigned int hits;
        float3 point;
        float3 normal;

        RayIntersection()
        : hits(0) {}

        RayIntersection(int hits)
        : hits(hits) {}

        RayIntersection(float3 _point, float3 _normal)
        : hits(1), point(_point), normal(_normal) {}

        RayIntersection(unsigned int hits, float3 point, float3 normal)
        : hits(hits), point(point), normal(normal) {}
    };

    struct Ray {
        float3 p, d;

        Ray(float3 p, float3 d)
        : p(p), d(d) {}
    };

    RayIntersection Raycast(const Ray &ray, const Triangle &tri) {
        float3 point;
        float t = dot((tri.a-ray.p), tri.getNormal())/(dot(tri.getNormal(), ray.d));
        if (t < 0) {
            return RayIntersection(0);
        }
        point = ray.p + ray.d*t;

        float3 triNormal = tri.getNormal();

        float3 AB = tri.b-tri.a;
        float3 BC = tri.c-tri.b;
        float3 CA = tri.a-tri.c;

        float3 AP = point - tri.a;
        float3 BP = point - tri.b;
        float3 CP = point - tri.c;

        bool ATest = dot(cross(AB,AP), triNormal) >= 0.0f;
        bool BTest = dot(cross(BC,BP), triNormal) >= 0.0f;
        bool CTest = dot(cross(CA,CP), triNormal) >= 0.0f;
        if (ATest && BTest && CTest) {
            return RayIntersection(point, tri.getNormal());
        }
        else {
            return RayIntersection(0);
        }
    }

    RayIntersection Raycast(const Ray &ray, const std::vector<Triangle> &tris) {
        RayIntersection result(0);
        int hits = 0;
        for (int i = 0; i < tris.size(); i++) {
            float3 point;
            Triangle tri = tris[i];
            float t = dot((tri.a-ray.p), tri.getNormal())/(dot(tri.getNormal(), ray.d));
            if (t <= 0) {
                continue;
            }
            point = ray.p + ray.d*t;

            float3 triNormal = tri.getNormal();

            float3 AB = tri.b-tri.a;
            float3 BC = tri.c-tri.b;
            float3 CA = tri.a-tri.c;

            float3 AP = point - tri.a;
            float3 BP = point - tri.b;
            float3 CP = point - tri.c;

            bool ATest = dot(cross(AB,AP), triNormal) >= 0.0f;
            bool BTest = dot(cross(BC,BP), triNormal) >= 0.0f;
            bool CTest = dot(cross(CA,CP), triNormal) >= 0.0f;

            if (ATest && BTest && CTest) {
                hits++;
                if (result.hits) {
                    float currentDist = bMath::distance(ray.p, result.point);
                    float newDist = bMath::distance(ray.p, point);
                    if (newDist < currentDist) {result = RayIntersection(point, tri.getNormal());}
                    else {result.hits = hits;}
                }
                else {
                    result = RayIntersection(hits, point, tri.getNormal());
                }
            }
            else {
                continue;
            }
        }
        return result;
    }

    
}

#endif