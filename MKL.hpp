#ifndef MKL_HPP_BY_TANMAY_SARTHAK
#define MKL_HPP_BY_TANMAY_SARTHAK

#include"Matrix.hpp"
#include<mkl/mkl.h>

// Mat3 += Mat1 * Mat2
template<class __num>
void addProductMKL(Matrix<__num> &Mat1, Matrix<__num> &Mat2, Matrix<__num> &Mat3){
    // If dimensions don't match then throw error
    if (Mat1.m != Mat2.n)
        throw std::invalid_argument("Invalid dimensions of matrices for multiplication.");
    // If dimensions don't match then throw error
    if (Mat3.n != Mat1.n || Mat3.m != Mat2.m) 
        throw std::invalid_argument("Invalid dimensions of matrices for addition");

    // create A, B, C pointers for mkl
    double *A, *B, *C;
    // C = alpha * A * B + beta * C
    double alpha = 1.0, beta = 1.0;
    A = (double*) mkl_malloc(Mat1.n * Mat1.m * sizeof(double), 64);
    B = (double*) mkl_malloc(Mat2.n * Mat2.m * sizeof(double), 64);
    C = (double*) mkl_malloc(Mat3.n * Mat3.m * sizeof(double), 64);

    // Assign value to A
    for (int i = 0; i < Mat1.n; i++){
        for (int j = 0; j < Mat1.m; j++){
            A[Mat1.m * i + j] = Mat1.mat[i][j];
        }
    }

    // Assign values to B
    for (int i = 0; i < Mat2.n; i++){
        for (int j = 0; j < Mat2.m; j++){
            B[Mat2.m * i + j] = Mat2.mat[i][j];
        }
    }

    // Assign values to C
    for (int i = 0; i < Mat3.n; i++){
        for (int j = 0; j < Mat3.m; j++){
            C[Mat3.m * i + j] = Mat3.mat[i][j];
        }
    }

    // Calculate C = alpha * A * B + beta * C
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, Mat1.n, Mat2.m, Mat1.m, alpha, A, Mat1.m, B, Mat2.m, beta, C, Mat2.m);
    // Assign values to matrix
    for (int i = 0; i < Mat3.n; i++){
        for (int j = 0; j < Mat3.m; j++){
            Mat3.mat[i][j] = C[Mat3.m * i + j];
        }
    }
}

#endif
