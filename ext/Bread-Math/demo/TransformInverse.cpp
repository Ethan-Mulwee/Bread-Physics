#include "bMath/matrix.hpp"
#include "bMath/ext/iostream.hpp"

using namespace bMath;

int main() {
    Matrix<float,3,1> m1(
        4,
        5,
        6
    );

    Matrix<float,1,3> m2(
        -4,-5,-6
    );

    std::cout << m2*m1 << "\n";
}