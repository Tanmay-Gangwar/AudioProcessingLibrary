main.cpp consists of 

    void throwInvalidFileError(std::string fileName)
        throws file does not exist exception


    void processFullyConnected(char *inputMatrixFile, char *weightMatrixFile, char *biasMatrixFile, char *outputMatrixFile, std::string library)
        inputs inputMatrix, weightMatrix, biasMatrix
        outputMatrix = inputMatrix * weightMatrix + biasMatrix
        outputs outputMatrix
    [TERMINAL COMMAND:] ./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt [LIBRARY]
    [LIBRARY] = pthread / normal / mkl / openblas


    void processActivation(std::string activationType, char *inputMatrixFile, char *outputMatrixFile)
        inputs matrx
        apply activation function accordingly
        outputs matrix
    [TERMINAL COMMAND:] ./yourcode.out activation [ACTIVATION TYPE] inputmatrix.txt outputmatrix.txt
    [ACTIVATION TYPE] = relu / tanh


    void processPooling(std::string poolingType, char *inputMatrixFile, const char* stride, char *outputMatrixFile)
        input matrix
        apply pooling accordingly
        outputs matrix
    [TERMINAL COMMAND:] ./yourcode.out pooling [POOLING TYPE] inputmatrix.txt [STRIDE] outputmatrix.txt
    [POOLING TYPE] = max / average
    [STRIDE] = (integer)length of stride


    void processProbability(std::string probabilityType, char *inputVectorFile, char *outputVectorFile)
        inputs vector
        apply probability accordingle
        outputs vector
    [TERMINAL COMMAND:] ./yourcode.out probability [PROBABILITY TYPE] inputmatrix.txt outputmatrix.txt
    [PROBABILITY TYPE] = softmax / sigmoid


    void help()
        outputs help on terminal
    [TERMINAL COMMAND:] ./yourcode.out help


Matrix.hpp consists of
class Matrix

    int n => represents number of rows
    int m => represents number of columns
    vector<vector> mat => represents Matrix

    operator+ => adds other matrix to self and returns their sum
    operator- => subtracts other matrix from self and returns their difference
    operator* => multiplies other matrix with self and return their product
    operator+= => adds other matrix to self
    operator-= => subtracts other matrix from self
    operator*= => multiplies other matri from self

    applyRelu => applies relu elementwise
    applyTanh => applies tanh elementwise

    applyMaxPool => creates a new matrix as a result of max pooling of a given stride on matrix
    applyAvgPool => creates a new matrix as a result of average pooling of a given stride on matrix

    productPthread => calls pthreadProduct with its respective argument
    pthreadProduct => 
    if threadDepth == 0:
        It only do normal matrix multiplication
    else:
        It breaks martices into 4 sub matrices and call itself recursively till threadDepth > 0. Finally it merges submatrices into one.

    istream &operator>> => Inputs matrix
    ostream &operator<< => Outputs matrix


Vector.hpp consists of
class Vector

    int n => represents number of elements in vector
    vector vec => represents vector

    applySoftMax => applies softMax function on elements of vector
    applySigmoid => applies sigmoid function on elements of vector


    istream &operator>> => Inputs vector
    ostream &operator<< => Outputs vector

MKL.hpp and OpenBlas.hpp both consists of

    addProduct function for Mat3 += Mat1 * Mat2 using cblas_degmm function.

measure.cpp

    to_run vector takes input the sizes of test cases to run mkl, openblas and pthread library functions implemented in main.cpp
    For every size input, it generates a matrix filled with random floats in range [-10.0, 10.0].
    Then it performs matrix multiplication 10 times and calculates mean and standard deviation of the run times for all libraries.
    All these data set formed are saved as 
        normalData.txt for no library used
        pthreadData.txt for using threading
        mklData.txt for using MKL library
        openBlasData.txt for using Openblas library
    these data set consist of n (to_run.size()) rows and 4 columns
        1st column represent size of matrix it ran on
        2nd column represent average time taken for multiplication
        3rd column represent average error in times
        4th column represent standard deviation for the corresponding time


How to install mkl library in linux (ubuntu)

    sudo apt-get update
    sudo apt-get install intel-mkl-full


How to install openblas library in linux (ubuntu)

    sudo apt-get update
    sudo apt-get install libopenblas-dev


For compilation

    g++ -fopenmp main.cpp -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lopenblas -lpthread -o yourcode.out


GNUPlotScript.p consists of

    gnuplotscript commands to plot graph of time-taken to run for different matrix size in case of no library used, pthread, mkl and openblas library used.
    

script.sh consists of

    command to run measure.cpp and create new gnuplot