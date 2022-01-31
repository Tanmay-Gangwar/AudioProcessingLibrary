#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>
#include<cmath>

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
};


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