#ifndef BMATH_MATRIX
#define BMATH_MATRIX

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

namespace bMath {
template <typename T, int rows, int cols> struct Matrix {
  T data[rows][cols] = {};

  Matrix() {}

  Matrix(T data[rows][cols]) : data(data) {}

  template <typename... Args> Matrix(Args... args) : data{(T)args...} {}

  // TODO:

  T* operator[](const int i) const { return data[i]; }

  T& operator() (const int i, const int j) {return data[i][j];}

  T operator() (const int i, const int j) const {return data[i][j];}

  // Return component wise addition of two matrices
  Matrix<T, rows, cols> operator+(const Matrix<T, rows, cols> &m) const {
    Matrix<T, rows, cols> newMat;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        newMat.data[i][j] = data[i][j] + m.data[i][j];
      }
    }
    return newMat;
  }

  template<int Mrows, int Mcols>
  Matrix<T, rows, Mcols> operator*(Matrix<T, Mrows, Mcols> &m) {
    Matrix<T, rows, Mcols> result;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < Mcols; j++) {
        float sum = 0;
        for (int k = 0; k < cols; k++) {
          sum += (*this)(i,k)*m(k,j);
        }
        result(i,j) = sum;
      }
    }
    return result;
  }
};

typedef Matrix<float, 3, 3> Matrix3;
typedef Matrix<float, 4, 4> Matrix4;

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
      os << m(i,j) << ((j < cols-1) ? ", " : "");
    }
   os << "|" << "\n";
  }
 return os;
}

// TODO: temp function for inversion look into how blender does matrix inversion on n size matriices
Matrix3 invert(Matrix3 &m) {

}

Matrix4 invert(Matrix4 &m) {

}


} // namespace bMath

#endif
