#ifndef MATRIX_HPP_BY_TANMAY_SARTHAK
#define MATRIX_HPP_BY_TANMAY_SARTHAK

#include<vector>
#include<cmath>
#include<pthread.h>

// Pthread only allowes to pass one argument of type void* therefore creating structure to pass it as argument for pthread
template<class __num>
struct pthreadArgs{
    std::vector<std::vector<__num>> *A, *B, *C;
    int threadDepth;
    pthreadArgs(std::vector<std::vector<__num>> *A, std::vector<std::vector<__num>> *B, std::vector<std::vector<__num>> *C, int threadDepth){
        this->A = A;
        this->B = B;
        this->C = C;
        this->threadDepth = threadDepth;
    }
    pthreadArgs(){}
};

template<class __num>
void* pthreadProduct(void* arg);

// Matrix class for n rows and m columns
template<class __num>
class Matrix{
    public:

    int n, m;
    std::vector<std::vector<__num>> mat;

    // Initialize matrix
    Matrix(int n = 0, int m = 0){
        this->n = n;
        this->m = m;
        mat.resize(n, std::vector<__num>(m));
    }


    Matrix operator+(Matrix &other){
        // If dimensions don't match then throw error
        if (n != other.n || m != other.m) 
            throw std::invalid_argument("Invalid dimensions of matrices for addition");
        
        Matrix<__num> res(n, m);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++)
                res.mat[i][j] = mat[i][j] + other.mat[i][j];    // elementwise addition
        }
        return res;
    }


    Matrix operator-(Matrix &other){
        // If dimensions don't match then throw error
        if (n != other.n || m != other.m) 
            throw std::invalid_argument("Invalid dimensions of matrices for subtraction");
        
        Matrix<__num> res(n, m);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++)
                res.mat[i][j] = mat[i][j] - other.mat[i][j];    // elementwise subtraction
        }
        return res;
    }


    Matrix operator*(Matrix &other){
        // If dimensions don't match then throw error
        if (m != other.n)
            throw std::invalid_argument("Invalid dimensions of matrices for multiplication.");
        
        Matrix<__num> res(n, other.m);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < other.m; j++){
                res.mat[i][j] = 0;
                for (int k = 0; k < m; k++)
                    res.mat[i][j] += mat[i][k] * other.mat[k][j];
            }
        }
        return res;
    }


    void operator+=(Matrix &other){
        // If dimensions don't match then throw error
        if (n != other.n || m != other.m) 
            throw std::invalid_argument("Invalid dimensions of matrices for addition");
        
        for (int i = 0; i < n; i++){
            for (int j = 0; j < other.m; j++)
                mat[i][j] += other.mat[i][j];
        }
    }


    void operator-=(Matrix &other){
        // If dimensions don't match then throw error
        if (n != other.n || m != other.m) 
            throw std::invalid_argument("Invalid dimensions of matrices for subtraction");
        
        for (int i = 0; i < n; i++){
            for (int j = 0; j < other.m; j++)
                mat[i][j] -= other.mat[i][j];
        }
    }


    void operator*=(Matrix &other){
        // If dimensions don't match then throw error
        if (m != other.n)
            throw std::invalid_argument("Invalid dimensions of matrices for multiplication.");
        
        // Create a 2-D vector to store multiplication result
        std::vector<std::vector<__num>> res(n, std::vector<__num>(other.m, 0.0f));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < other.m; j++){
                for (int k = 0; k < m; k++)
                    res[i][j] += mat[i][k] * other.mat[k][j];
            }
        }

        // Copy the resulting product to change self matrix
        m = other.m;
        mat.resize(n, std::vector<__num>(m));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++)
                mat[i][j] = res[i][j];
        }
    }


    // apply relu function elementwise
    void applyRelu(){
        for (auto &row: mat){
            for (__num &x: row){
                if (x < 0) x = 0;
            }
        }
    }


    // apply tanh function elementwise
    void applyTanh(){
        for (auto &row: mat){
            for (__num &x: row){    
                // As exp(x) can lead to overflow therefre trying to find exp of negative number
                if (x < 0){
                    __num temp = exp(x * 2);
                    x = (temp - 1) / (temp + 1);        // tanh(x) = (exp(2x) - 1) / (exp(2x) + 1)
                }
                else{
                    __num temp = exp(-x * 2);
                    x = (1 - temp) / (1 + temp);        // tanh(x) = (1-exp(-2x)) / (1 + exp(-2x))
                }
            }
        }
    }


    // apply maxPool on matrix
    Matrix maxPool(int stride){
        // Create matrix of dimension ceil(n / stride), ceil(m / stride)
        Matrix<__num> res((n + stride - 1) / stride, (m + stride - 1) / stride);
        for (int i = 0; i < res.n; i++){
            for (int j = 0; j < res.m; j++){
                res.mat[i][j] = mat[stride * i][stride * j];

                int limK = stride * (i + 1);
                if (n < limK) limK = n;

                int limL = stride * (j + 1);
                if (m < limL) limL = m;

                // res.mat[i][j] = max(mat[stride * i ... limK][stride * j ... limL])
                for (int k = stride * i; k < limK; k++){
                    for (int l = stride * j; l < limL; l++){
                        if (mat[k][l] > res.mat[i][j]) 
                            res.mat[i][j] = mat[k][l];
                    }
                }
            }
        }
        return res;
    }


    // apply avgPool on matrix
    Matrix avgPool(int stride){
        // Create matrix of dimension ceil(n / stride), ceil(m / stride)
        Matrix<__num> res((n + stride - 1) / stride, (m + stride - 1) / stride);
        for (int i = 0; i < res.n; i++){
            for (int j = 0; j < res.m; j++){
                res.mat[i][j] = 0.0f;
                
                int limK = stride * (i + 1);
                if (n < limK) limK = n;

                int limL = stride * (j + 1);
                if (m < limL) limL = m;

                // res.mat[i][j] = sum(mat[stride * i ... limK][stride * j ... limL]) / number of elements
                for (int k = stride * i; k < limK; k++){
                    for (int l = stride * j; l < limL; l++){
                        res.mat[i][j] += mat[k][l];
                    }
                }
                res.mat[i][j] /= (limK - stride * i) * (limL - stride * j);
            }
        }
        return res;
    }

    // Returns product of other and self using threads
    Matrix productPthread(Matrix &other, int threadDepth){
        Matrix res(n, other.m);
        pthreadArgs<__num>* args = new pthreadArgs<__num>(&mat, &other.mat, &res.mat, threadDepth);
        pthreadProduct<__num>(args);
        return res;
    }
};

// Takes argument in form of pointer to the structure
template<class __num>
void* pthreadProduct(void* arg){
    pthreadArgs<__num>* args = (pthreadArgs<__num>*) arg;
    int n = args->A->size();
    int k = args->B->size();
    int m = args->B->at(0).size();
    // Perform basic matrix multiplication
    if (args->threadDepth == 0 || n == 1 || k == 1 || m == 1){
        args->C->resize(n, std::vector<__num>(m));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                for (int x = 0; x < k; x++){
                    args->C->at(i)[j] += args->A->at(i)[x] * args->B->at(x)[j];
                }
            }
        }
        return NULL;
    }

    // Divides A into 4 sub-matrices
    Matrix<__num> A1(n / 2, k / 2), A2(n / 2, (k + 1) / 2), A3((n + 1) / 2, k / 2), A4((n + 1) / 2, (k + 1) / 2);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < k; j++){
            if (i < n / 2){
                if (j < k / 2) A1.mat[i][j] = args->A->at(i)[j];
                else A2.mat[i][j - k / 2] = args->A->at(i)[j];
            }
            else{
                if (j < k / 2) A3.mat[i - n / 2][j] = args->A->at(i)[j];
                else A4.mat[i - n / 2][j - k / 2] = args->A->at(i)[j];
            }
        }
    }

    // Divides B into 4 sub-matrices
    Matrix<__num> B1(k / 2, m / 2), B2(k / 2, (m + 1) / 2), B3((k + 1) / 2, m / 2), B4((k + 1) / 2, (m + 1) / 2);
    for (int i = 0; i < k; i++){
        for (int j = 0; j < m; j++){
            if (i < k / 2){
                if (j < m / 2) B1.mat[i][j] = args->B->at(i)[j];
                else B2.mat[i][j - m / 2] = args->B->at(i)[j];
            }
            else{
                if (j < m / 2) B3.mat[i - k / 2][j] = args->B->at(i)[j];
                else B4.mat[i - k / 2][j - m / 2] = args->B->at(i)[j];
            }
        }
    }

    // Divides C as products of 4 sub-matrices of A and B
    Matrix<__num> C11, C12, C21, C22, C31, C32, C41, C42;
    pthreadArgs<__num>* threadArgs[8];
    threadArgs[0] = new pthreadArgs<__num>(&A1.mat, &B1.mat, &C11.mat, args->threadDepth - 1);
    threadArgs[1] = new pthreadArgs<__num>(&A2.mat, &B3.mat, &C12.mat, args->threadDepth - 1);
    threadArgs[2] = new pthreadArgs<__num>(&A1.mat, &B2.mat, &C21.mat, args->threadDepth - 1);
    threadArgs[3] = new pthreadArgs<__num>(&A2.mat, &B4.mat, &C22.mat, args->threadDepth - 1);
    threadArgs[4] = new pthreadArgs<__num>(&A3.mat, &B1.mat, &C31.mat, args->threadDepth - 1);
    threadArgs[5] = new pthreadArgs<__num>(&A4.mat, &B3.mat, &C32.mat, args->threadDepth - 1);
    threadArgs[6] = new pthreadArgs<__num>(&A3.mat, &B2.mat, &C41.mat, args->threadDepth - 1);
    threadArgs[7] = new pthreadArgs<__num>(&A4.mat, &B4.mat, &C42.mat, args->threadDepth - 1);

    // create threads
    pthread_t thread[8];
    for (int i = 0; i < 8; i++){
        pthread_create(&thread[i], NULL, pthreadProduct<__num>, threadArgs[i]);
    }

    // wait till all the threads are completed
    for (int i = 0; i < 8; i++){
        pthread_join(thread[i], NULL);
    }

    // Combines C11 to C42 to form result C
    args->C->resize(n, std::vector<__num>(m));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (i < n / 2){
                if (j < m / 2) args->C->at(i)[j] = C11.mat[i][j] + C12.mat[i][j];
                else args->C->at(i)[j] = C21.mat[i][j - m / 2] + C22.mat[i][j - m / 2];
            }
            else{
                if (j < m / 2) args->C->at(i)[j] = C31.mat[i - n / 2][j] + C32.mat[i - n / 2][j];
                else args->C->at(i)[j] = C41.mat[i - n / 2][j - m / 2] + C42.mat[i - n / 2][j - m / 2];
            }
        }
    }

    return NULL;
}


// Input Matrix columnwise
template<class __num>
std::istream &operator>>(std::istream &in, Matrix<__num> &matrix){
    for (int j = 0; j < matrix.m; j++){
        for (int i = 0; i < matrix.n; i++)
            in >> matrix.mat[i][j];
    }
    return in;
}


// Output matrix columnwise
template<class __num>
std::ostream &operator<<(std::ostream &out, Matrix<__num> &matrix){
    for (int j = 0; j < matrix.m; j++){
        for (int i = 0; i < matrix.n; i++)
            out << matrix.mat[i][j] << "\n";
    }
    return out;
}
#endif