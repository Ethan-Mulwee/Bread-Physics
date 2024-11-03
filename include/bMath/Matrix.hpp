#ifndef BMATH_MATRIX
#define BMATH_MATRIX

#include <assert.h>
#include <iostream>

namespace bMath {
    template <int rows, int cols>
    struct Matrix {
        float data[rows][cols] = {};
        const static int r = rows; const static int c = cols;

        Matrix() {}

        Matrix(float data[rows][cols]) : data(data) {}

        Matrix operator+(const Matrix &m) const {
            Matrix<rows,cols> newMat;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    newMat.data[i][j] = data[i][j] + m.data[i][j];
                }
            }
            return newMat;
        }

        template <typename T>
        auto operator*(const T &m) const {
            assert(cols == m.r);
            Matrix<rows,m.c> newMat;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < m.c; j++) {
                    float total = 0;
                    for (int k = 0; k < cols; k++) {
                        total += data[i][k]*m.data[k][i];
                    }
                    newMat.data[i][j] = total;
                }
            }
            return newMat;
        }



        void log() const {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    std::cout << data[i][j] << ", ";
                }
                std:: cout << "\n";
            }
        }
    };

    template <int rows, int cols>
    std::ostream& operator<<(std::ostream& os, const Matrix<rows,cols> &m) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                os << m.data[i][j] << ", ";
            }
            os << "\n";
        }
        return os;
    }    
}

#endif