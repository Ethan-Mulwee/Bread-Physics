#include "bMath/bMath.hpp"
#include "bMath/ext/iostream.hpp"
#include <chrono>

using namespace bMath;

// g++ VectorTransformation.cpp -I../include -O2

int main() {
  // Test for transforming a vector3 by a matrix 3x3
  
  matrix3 m1(
    2,0,0,
    0,2,0,
    0,0,2
  );

  float3 v1(4,5,6);

  std::cout << "Matrix: \n" << m1 << "\n";
  std::cout << "Vector: " << v1 << "\n"; 
  std::cout << "Transformed Vector: " << (v1*m1) << "\n";

  // Test for transforming a vector3 by a matrix 4x4

  matrix4 m2(
    2,0,0,1,
    0,2,0,1,
    0,0,2,1,
    0,0,0,1
  );

  float3 v2(4,5,6);

  std::cout << "Matrix: \n" << m2 << "\n";
  std::cout << "Vector: " << v2 << "\n"; 
  std::cout << "Transformed Vector: " << (v2*m2) << "\n";

  // Test for transforming a vector4 by a matrix 4x4
  
  matrix4 m3(
    2,0,0,1,
    0,2,0,1,
    0,0,2,1,
    0,0,0,1
  );

  float4 v3(4,5,6,1);

  std::cout << "Matrix: \n" << m3 << "\n";
  std::cout << "Vector: " << v3 << "\n"; 
  std::cout << "Transformed Vector: " << (v3*m3) << "\n";

  // Performance testing

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000000; i++) {
    v3*m2;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}