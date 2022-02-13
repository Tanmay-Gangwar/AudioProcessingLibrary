#include<iostream>
#include"Vector.hpp"
#include"Matrix.hpp"
#include"MKL.hpp"
#include"OpenBlas.hpp"

// If file does not exist throw error that file doesn't exist
void throwInvalidFileError(std::string fileName){
    std::string error = fileName + " does not exist\n FOR HELP TYPE\n ./yourcode.out help";
    throw std::invalid_argument(error);
}

void processFullyConnectedPthread(Matrix<float> &inputMatrix, Matrix<float> &weightMatrix, Matrix<float> &biasMatrix, char *outputMatrixFile){
    // outputMatrix = inputMatrix * weightMatrix + biasMatrix
    Matrix<float> outputMatrix = inputMatrix.productPthread(weightMatrix, 5);
    outputMatrix += biasMatrix;

    // Output outputMatrix
    freopen(outputMatrixFile, "w", stdout);
    std::cout << outputMatrix.m << "\n" << outputMatrix.n << "\n";
    std::cout << outputMatrix;
    fclose(stdout);
}


void processFullyConnectedMKL(Matrix<float> &inputMatrix, Matrix<float> &weightMatrix, Matrix<float> &biasMatrix, char *outputMatrixFile){
    // biasMatrix += inputMatrix * weightMatrix
    addProductMKL(inputMatrix, weightMatrix, biasMatrix);

    // Output updated biasMatrix as it is the final result
    freopen(outputMatrixFile, "w", stdout);
    std::cout << biasMatrix.m << "\n" << biasMatrix.n << "\n";
    std::cout << biasMatrix;
    fclose(stdout);
}


void processFullyConnectedOpenBlas(Matrix<float> &inputMatrix, Matrix<float> &weightMatrix, Matrix<float> &biasMatrix, char *outputMatrixFile){
    // biasMatrix += inputMatrix * weightMatrix
    addProductOpenBlas(inputMatrix, weightMatrix, biasMatrix);

    // Output updated biasMatrix as it is the final result
    freopen(outputMatrixFile, "w", stdout);
    std::cout << biasMatrix.m << "\n" << biasMatrix.n << "\n";
    std::cout << biasMatrix;
    fclose(stdout);
}


void processFullyConnectedNormal(Matrix<float> &inputMatrix, Matrix<float> &weightMatrix, Matrix<float> &biasMatrix, char *outputMatrixFile){
    // outputMatrix = inputMatrix * weightMatrix + biasMatrix
    Matrix<float> outputMatrix = inputMatrix * weightMatrix;
    outputMatrix += biasMatrix;

    // Output outputMatrix
    freopen(outputMatrixFile, "w", stdout);
    std::cout << outputMatrix.m << "\n" << outputMatrix.n << "\n";
    std::cout << outputMatrix;
    fclose(stdout);
}

// Process library type and redirect to valid processFullyConnected
void processFullyConnected(char *inputMatrixFile, char *weightMatrixFile, char *biasMatrixFile, char *outputMatrixFile, std::string library){
    int n, m;
    FILE *file = freopen(inputMatrixFile, "r", stdin);
    if (!file) throwInvalidFileError(inputMatrixFile);
    // Input inputMatrix
    std::cin >> m >> n;
    Matrix<float> inputMatrix(n, m);
    std::cin >> inputMatrix;
    fclose(stdin);

    file = freopen(weightMatrixFile, "r", stdin);
    if (!file) throwInvalidFileError(weightMatrixFile);
    // Input weight matrix
    std::cin >> m >> n;
    Matrix<float> weightMatrix(n, m);
    std::cin >> weightMatrix;
    fclose(stdin);

    file = freopen(biasMatrixFile, "r", stdin);
    if (!file) throwInvalidFileError(biasMatrixFile);
    // Input biasMatrix
    std::cin >> m >> n;
    Matrix<float> biasMatrix(n, m);
    std::cin >> biasMatrix;
    fclose(stdin);

    if (library == "mkl") processFullyConnectedMKL(inputMatrix, weightMatrix, biasMatrix, outputMatrixFile);
    else if (library == "openblas") processFullyConnectedOpenBlas(inputMatrix, weightMatrix, biasMatrix, outputMatrixFile);
    else if (library == "pthread") processFullyConnectedPthread(inputMatrix, weightMatrix, biasMatrix, outputMatrixFile);
    else if (library == "normal") processFullyConnectedNormal(inputMatrix, weightMatrix, biasMatrix, outputMatrixFile);
    else throw std::invalid_argument("FOR HELP TYPE \n ./yourcode.out help");
}


void processActivation(std::string activationType, char *inputMatrixFile, char *outputMatrixFile){
    int n, m;
    FILE *file = freopen(inputMatrixFile, "r", stdin);
    if (!file) throwInvalidFileError(inputMatrixFile);
    // input matrix
    std::cin >> m >> n;
    Matrix<float> matrix(n, m);
    std::cin >> matrix;
    fclose(stdin);
    
    // apply activation function or throw error if it's invalid
    if (activationType == "relu") matrix.applyRelu();
    else if (activationType == "tanh") matrix.applyTanh();
    else throw std::invalid_argument("invalid activation function.\n FOR HELP TYPE\n ./yourcode.out help");

    // output matrix
    freopen(outputMatrixFile, "w", stdout);
    std::cout << m << "\n" << n << "\n";
    std::cout << matrix;
    fclose(stdout);
}


void processPooling(std::string poolingType, char *inputMatrixFile, const char* stride, char *outputMatrixFile){
    int n, m;
    FILE *file = freopen(inputMatrixFile, "r", stdin);
    if (!file) throwInvalidFileError(inputMatrixFile);
    // input inputMatrix
    std::cin >> m >> n;
    Matrix<float> inputMatrix(n, m);
    std::cin >> inputMatrix;
    fclose(stdin);

    // apply relevant pooling or throw exception if it's invalid
    Matrix<float> outputMatrix;
    if (poolingType == "max") outputMatrix = inputMatrix.maxPool(atoi(stride));
    else if (poolingType == "average") outputMatrix = inputMatrix.avgPool(atoi(stride));
    else throw std::invalid_argument("invalid pooling type.\n FOR HELP TYPE\n ./yourcode.out help");


    m = outputMatrix.m;
    n = outputMatrix.n;
    // output outputMatrix
    freopen(outputMatrixFile, "w", stdout);
    std::cout << m << "\n" << n << "\n";
    std::cout << outputMatrix;
    fclose(stdout);
}


void processProbability(std::string probabilityType, char *inputVectorFile, char *outputVectorFile){
    int n;
    FILE *file = freopen(inputVectorFile, "r", stdin);
    if (!file) throwInvalidFileError(inputVectorFile);
    // input vector
    std::cin >> n;
    Vector<float> vec(n);
    std::cin >> vec;
    fclose(stdin);

    // apply relevant probability or throw exception if it's invalid
    if (probabilityType == "softmax") vec.applySoftmax();
    else if (probabilityType == "sigmoid") vec.applySigmoid();
    else throw std::invalid_argument("Invalid probability type.\n FOR HELP TYPE\n ./yourcode.out help");

    // output vector
    freopen(outputVectorFile, "w", stdout);
    std::cout << n << "\n";
    std::cout << vec;
    fclose(stdout);
}


void help(){
    std::cout << "Valid inputs:\n";
    std::cout << "To run fullyConnected function on matrix:\n";
    std::cout << "./yourcode.out fullyconnected inputmmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt [LIBRARY]\n";
    std::cout << "[LIBRARY] = mkl / openblas / pthread / normal\n";
    std::cout << "To run activation function on matrix:\n";
    std::cout << "./yourcode.out activation [ACTIVATION TYPE] inputmatrix.txt outputmatrix.txt\n";
    std::cout << "[ACTIVATION TYPE] = relu / tanh\n";
    std::cout << "To run pooling function on matrix:\n";
    std::cout << "./yourcode.out pooling [POOLING TYPE] inputmatrix.txt [STRIDE] outputmatrix.txt\n";
    std::cout << "[POOLING TYPE] = max / average\n";
    std::cout << "[STRIDE] = (integer)length of stride\n";
    std::cout << "To run probability function on a vector:\n";
    std::cout << "./yourcode.out probability [PROBABILITY TYPE] inputmatrix.txt outputmatrix.txt\n";
    std::cout << "[PROBABILITY TYPE] = softmax / sigmoid\n";
    std::cout << "For help:\n";
    std::cout << "./yourcode.out help\n";
}


int main(int argCount, char **args){

    if (argCount == 0) throw std::invalid_argument("FOR HELP TYPE \n ./yourcode.out help");
    std::string s = args[1];
    if (s == "fullyconnected" && argCount == 7) processFullyConnected(args[2], args[3], args[4], args[5], args[6]);
    else if (s == "activation" && argCount == 5) processActivation(args[2], args[3], args[4]);
    else if (s == "pooling" && argCount == 6) processPooling(args[2], args[3], args[4], args[5]);
    else if (s == "probability" && argCount == 5) processProbability(args[2], args[3], args[4]);
    else if (s == "help") help();
    else throw std::invalid_argument("FOR HELP TYPE \n ./yourcode.out help");

    return 0;
}