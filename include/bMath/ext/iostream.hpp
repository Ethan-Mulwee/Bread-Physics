#ifndef BMATH_IOSTREAM
#define BMATH_IOSTREAM

#include <iostream>
#include "../fwd.hpp"
#include <string>
#include <sstream>

namespace bMath {
// TODO: Find the biggest number in the matrix and add spaces accordingly so all rows are of equal length
template <typename T, int rows, int cols>
std::ostream &operator<<(std::ostream &os, const Matrix<T, rows, cols> &m) {
  // find max length
  // int maxlength = 0;
  // for (int k = 0; k < rows; k++) {
  //   for (int l = 0; l < cols; l++) {
  //     std::stringstream temp;
  //     temp << m(k,l);
  //     if (maxlength < temp.str().length()) {
  //        maxlength = temp.str().length();
  //     }
  //   }
  // }
  // std::cout << maxlength << "\n";
  // print matrix
  for (int i = 0; i < rows; i++) {
    os << "|";
   for (int j = 0; j < cols; j++) {
     // std::stringstream output;
     // output << m(i,j);
     // if (output.str().length() < maxlength) {
       // int amount = maxlength-output.str().length();
       // for (int p = 0; p < amount; p++) {
       //   output.str() = " " + output.str();
       // }
     // }
      os << m(i,j); 
      os << ((j < cols-1) ? ", " : "");
    }
   os << "|" << "\n";
  }
 return os;
}

template <typename T, int n>
std::ostream &operator<<(std::ostream &os, const Vector<T, n> &v) {
  os << "(";
  for (int i = 0; i < n; i++) {
    os << v[i];
    os << ((i != n - 1) ? ", " : "");
  }
  os << ")";
  return os;
}
} // namespace bMath

#endif