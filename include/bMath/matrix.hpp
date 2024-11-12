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
    for (int j = 0; j < cols; j++) {
      for (int i = 0; i < rows; i++) {
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
Matrix3 invert(const Matrix3 &m) {
 return m;
}

Matrix4 invert(const Matrix4 &m) {
  return m;
}

// TODO: figure out how to make this for loop logic happen at compile time
template<typename T, int rows, int cols>
Matrix<T, rows, cols> transpose(Matrix<T, rows, cols> &m) {
  Matrix<T, rows, cols> result;
  for (int j = 0; j < cols; j++) {
    for (int i = 0; i < rows; i++) {
      result(j,i) = m(i,j);
    }
  }
  return result;
}

Matrix3 transpose(const Matrix3 &m) {
  return Matrix3(
    m(0,0), m(1,0), m(2,0),
    m(0,1), m(1,1), m(2,1),
    m(0,2), m(1,2), m(2,2)
  );
}

Matrix4 transpose(const Matrix4 &m) {
  return Matrix4(
    m(0,0), m(1,0), m(2,0), m(3,0),
    m(0,1), m(1,1), m(2,1), m(3,1),
    m(0,2), m(1,2), m(2,2), m(3,2),
    m(0,3), m(1,3), m(2,3), m(3,3)
  );
}

// TODO: add determiant functions
float determinant(const Matrix3 &m) {
  return 0;
}

float determinant(const Matrix4 &m) {
  return 0;
}

// TODO:
// B^-1MB
// B is the transformation of the object aka the transformation that takes the coords from world space ot local space
Matrix4 ChangeBasis(const Matrix4 &m, const Matrix4 &b) {
  return Matrix4(
    
  );
}

// TODO: Creates a rotation matrix from euler angles in the order of xyz
Matrix3 rotationMatrix(float x, float y, float z) {
  return Matrix3();
}




} // namespace bMath

#endif
