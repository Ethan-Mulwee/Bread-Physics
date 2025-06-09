#include "bMath/bMath.hpp"
#include "bMath/ext/iostream.hpp"

using namespace bMath;

// g++ Quaternion.cpp -I../include

int main() {
    quaternion q((float)M_PI*0.5f, float3(0,1,0));
    quaternion p ((float)M_PI*0.7f, float3(0,1,0));

    // std::cout << rotationBetween(q,p) << "\n";

    quaternion g(M_PI/2, 0.2f, 0);

    std::cout << g << "\n";
}