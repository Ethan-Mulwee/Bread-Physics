#include "bMath/bMath.hpp"
#include "bMath/ext/iostream.hpp"

// g++ Types.cpp -I../include

using namespace bMath;

int main() {
  Vector<int, 3> intvec1(1,2,3);
  std::cout << intvec1 << "\n";
  Vector<int, 3> intvec2(3,4,5);
  std::cout << intvec2 << "\n";

  std::cout << intvec1 + intvec2 << "\n";

  std::cout << intvec1.length() << "\n";
  std::cout << normalized(intvec1) << "\n";
  std::cout << normalized(intvec2) << "\n";

  std::cout << intvec1.x << "\n";
}