#include "bMath/bMath.hpp"
#include "bMath/ext/iostream.hpp"

// g++ MemoryTesting.cpp -I../include -g

using namespace bMath;

int main() {
  int test = 9;
  Vector<int, 3> test2 (4,5,6);
  Matrix<int, 3,3> test4(
    1,2,3,
    4,5,6,
    7,8,9
  );
  int test3 = 2;
}