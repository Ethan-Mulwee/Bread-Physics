#ifndef BMATH_MATRIX
#define BMATH_MATRIX

#include <assert.h>

namespace bMath {
template <typename T, int rows, int cols> struct Matrix {
  T data[rows][cols] = {};

  Matrix() {}

  Matrix(T data[rows][cols]) : data(data) {}

  // TODO: Constructor for creating matrices from vectors

  template <typename... Args> Matrix(Args... args) : data{(T)args...} {}

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
typedef Matrix<float, 3, 3> float3x3;
typedef Matrix<float, 4, 4> float4x4;

// TODO: add invert functions
Matrix3 invert(Matrix3 &m) {
 return m;
}

Matrix4 invert(Matrix4 &m) {
  return m;
}

// TODO: add transpose functions
Matrix3 transpose(Matrix3 &m) {
  return m;
}

Matrix4 transpose(Matrix4 &m) {
  return m;
}

// TODO: add determiant functions
float determinant(Matrix3 &m) {

}

float determinant(Matrix4 &m) {
  
}

// TODO: Creates a rotation matrix from euler angles in the order of xyz
Matrix3 rotationMatrix(float x, float y, float z) {
  return Matrix3();
}




} // namespace bMath

#endif
