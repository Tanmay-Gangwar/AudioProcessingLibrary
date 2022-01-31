main.cpp consists of 

    void throwInvalidFileError(std::string fileName)
        throws file does not exist exception


    void processFullyConnected(char *inputMatrixFile, char *weightMatrixFile, char *biasMatrixFile, char *outputMatrixFile)
        inputs inputMatrix, weightMatrix, biasMatrix
        outputMatrix = inputMatrix * weightMatrix + biasMatrix
        outputs outputMatrix
    [TERMINAL COMMAND:] ./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt


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