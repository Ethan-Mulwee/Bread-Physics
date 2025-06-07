#ifndef BMATH_RAYLIB
#define BMATH_RAYLIB

#include <raylib.h>
#include <rlgl.h>
#include "../vector.hpp"
#include "../matrix.hpp"

inline Vector3 ConvertRay(const bMath::Vector<float,3> &v) {
    return Vector3{v.x,v.y,v.z};
}

inline Matrix ConvertRay(const bMath::matrix4 &m) {
    return Matrix{
        m(0,0), m(0,1), m(0,2), m(0,3),
        m(1,0), m(1,1), m(1,2), m(1,3),
        m(2,0), m(2,1), m(2,2), m(2,3),
        m(3,0), m(3,1), m(3,2), m(3,3)
    };
}

inline Ray ConvertRay(const bMath::Ray &ray) {
    return Ray{ConvertRay(ray.p), ConvertRay(ray.d)};
}

inline bMath::float2 ConvertBread(const Vector2 &v) {
    return bMath::float2(v.x,v.y);
}

inline bMath::float3 ConvertBread(const Vector3 &v) {
    return bMath::float3(v.x,v.y,v.z);
}

inline bMath::matrix4 ConvertBread(const Matrix &m) {
    return bMath::matrix4(
        m.m0, m.m4, m.m8, m.m12,
        m.m1, m.m5, m.m9, m.m13,
        m.m2, m.m6, m.m10, m.m14,
        m.m3, m.m7, m.m11, m.m15
    );
}

inline bMath::Ray ConvertBread(const Ray &ray) {
    return bMath::Ray(ConvertBread(ray.position), ConvertBread(ray.direction));
}

inline void DrawVector(const bMath::float3 &v) {
    DrawLine3D(Vector3{0,0,0}, ConvertRay(v), WHITE);  
    DrawSphere(ConvertRay(v), 0.02f, WHITE);
}

inline void DrawVector(const bMath::float3 &v, float thickness) {
    rlSetLineWidth(thickness);
    DrawLine3D(Vector3{0,0,0}, ConvertRay(v), WHITE);  
    DrawSphere(ConvertRay(v), 0.02f*thickness, WHITE);
}

inline void DrawVector(const bMath::float3 &v, Color color) {
    DrawLine3D(Vector3{0,0,0}, ConvertRay(v), color);
    DrawSphere(ConvertRay(v), 0.02f, color);
}

inline void DrawVector(const bMath::float3 &v, Color color, float thickness) {
    rlSetLineWidth(thickness);
    DrawLine3D(Vector3{0,0,0}, ConvertRay(v), color);
    DrawSphere(ConvertRay(v), 0.02f*thickness, color);
}

inline void DrawVector(const bMath::float3 &start, const bMath::float3 &v) {
    DrawLine3D(ConvertRay(start), ConvertRay(start+v), WHITE);
    DrawSphere(ConvertRay(start+v), 0.02f, WHITE);
}

inline void DrawVector(const bMath::float3 &start, const bMath::float3 &v, float thickness) {
    rlSetLineWidth(thickness);
    DrawLine3D(ConvertRay(start), ConvertRay(start+v), WHITE);
    DrawSphere(ConvertRay(start+v), 0.02f*thickness, WHITE);

}

inline void DrawVector(const bMath::float3 &start, const bMath::float3 &v, Color color) {
    DrawLine3D(ConvertRay(start), ConvertRay(start+v), color);
    DrawSphere(ConvertRay(start+v), 0.02f, color);
}

inline void DrawVector(const bMath::float3 &start, const bMath::float3 &v, Color color, float thickness) {
    float before = rlGetLineWidth();
    rlSetLineWidth(thickness);
    DrawLine3D(ConvertRay(start), ConvertRay(start+v), color);
    DrawSphere(ConvertRay(start+v), 0.02f*thickness, color);
}

inline void DrawBasis(const bMath::matrix3 &m) {
    DrawVector(m.col(0), RED);
    DrawVector(m.col(1), GREEN);
    DrawVector(m.col(2), BLUE);
}

inline void DrawBasis(const bMath::matrix3 &m, float scale) {
    DrawVector(m.col(0)*scale, RED);
    DrawVector(m.col(1)*scale, GREEN);
    DrawVector(m.col(2)*scale, BLUE);
}

inline void DrawBasis(const bMath::matrix3 &m, bMath::float3 &position, float scale) {
    DrawVector(position, m.col(0)*scale, RED);
    DrawVector(position, m.col(1)*scale, GREEN);
    DrawVector(position, m.col(2)*scale, BLUE);
}

inline void DrawGraph(const float* points, unsigned pointCount, int positionX, int positionY) {
    Vector2 vecPoints[pointCount];
    for (int i = 0; i < pointCount; i++) {
        vecPoints[i] = Vector2{(float)i+positionX,-points[i]+positionY};
    }
    DrawLineStrip(vecPoints, pointCount, WHITE);
}

inline void DrawGraph(const float* points, unsigned pointCount, int positionX, int positionY, float scaleX, float scaleY) {
    Vector2 vecPoints[pointCount];
    for (int i = 0; i < pointCount; i++) {
        vecPoints[i] = Vector2{((float)i*scaleX)+positionX,(-points[i]*scaleY)+positionY};
    }
    DrawLineStrip(vecPoints, pointCount, WHITE);
}

inline void DrawGraph(const float* points, unsigned pointCount, int positionX, int positionY, int width, int height) {
    Vector2 vecPoints[pointCount];
    float maxX = pointCount;
    float maxY = 0;
    for (int i = 0; i < pointCount; i++) {
        if (points[i] > maxY) maxY = points[i];
    }
    for (int i = 0; i < pointCount; i++) {
        vecPoints[i] = Vector2{((float)i*(width/maxX))+positionX,(-points[i]*(height/maxY))+positionY};
    }
    DrawLineStrip(vecPoints, pointCount, WHITE);
}

inline void DrawViewport3D(float axisLength, int gridSlices, float gridSpacing, float lineThickness) {
    rlSetLineWidth(lineThickness);
    DrawGrid(gridSlices,gridSpacing);
    DrawLine3D(Vector3{-axisLength,0,0}, Vector3{axisLength,0,0}, RED);
    DrawLine3D(Vector3{0,-axisLength,0}, Vector3{0,axisLength,0}, GREEN);
    DrawLine3D(Vector3{0,0,-axisLength}, Vector3{0,0,axisLength}, BLUE);
}

#endif