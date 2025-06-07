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
    os << "\n";
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

  template <typename T, std::size_t N>
  std::ostream &operator<<(std::ostream &os, const Vector<T, N> &v) {
    os << "(";
    for (int i = 0; i < N; i++) {
      os << v[i];
      os << ((i != N - 1) ? ", " : "");
    }
    os << ")";
    return os;
  }

  template <typename T>
  std::ostream &operator<<(std::ostream &os, const Quaternion<T> &q) {
    os << "(";
    os << q.w << ", " << q.x << ", " << q.y << ", " << q.z;
    os << ")";
    return os;
  }
} // namespace bm

#endif