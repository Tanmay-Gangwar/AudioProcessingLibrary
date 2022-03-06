#ifndef VECTOR_HPP_BY_TANMAY_SARTHAK
#define VECTOR_HPP_BY_TANMAY_SARTHAK

#include<vector>
#include<cmath>


template<class __num>
class Vector{
    public:
    int n;
    std::vector<__num> vec;

    // Initialize vector of size n
    Vector(int n = 0){
        this->n = n;
        vec.resize(n);
    }


    // applying softmax on every element
    // softmax(a[i]) = exp(a[i]) / (sum(exp(a[1]) to exp(a[n])))
    // To prevent any overflow, maxVal = max(a[1...n])
    // softmax(a[i]) = exp(a[i] - maxVal) / sum(exp(a[1] - maxVal) to exp(a[n] - maxVal))
    void applySoftmax(){
        __num maxVal = vec[0];
        for (__num &x: vec){
            if (x > maxVal) maxVal = x;
        }
        __num sum = 0;
        for (__num &x: vec) 
            sum += exp(x - maxVal);
        for (__num &x: vec) x = exp(x - maxVal) / sum;
    }


    // apply sigmoid termwise
    void applySigmoid(){
        for (__num &x: vec){
            if (x > 0){
                __num temp = exp(-x);
                x = (__num) 1 / (temp + 1);     // sigmoid(x) = 1 / (1 + exp(-x))
            }
            else{
                __num temp = exp(x);
                x = temp / (temp + 1);          // sigmoig(x) = exp(x) / (1 + exp(x))
            }
        }
    }
};


// input elements of vector
template<class __num>
std::istream &operator>>(std::istream &in, Vector<__num> &vec){
    for (__num &x: vec.vec) in >> x;
    return in;
}


// output element of vector
template<class __num>
std::ostream &operator<<(std::ostream &out, Vector<__num> &vec){
    for (__num &x: vec.vec) out << x << "\n";
    return out;
}
#endif