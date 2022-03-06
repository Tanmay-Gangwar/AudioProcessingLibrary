#include"audio.hpp"

// If file does not exist throw error that file doesn't exist
void throwInvalidFileError(std::string fileName){
    std::string error = fileName + " does not exist\n FOR HELP TYPE\n ./yourcode.out help";
    throw std::invalid_argument(error);
}

Matrix<float> convertRowMajorToMatrix(float *arr, int n, int m){
    Matrix<float> matrix(n, m);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            matrix.mat[i][j] = arr[m * i + j];
        }
    }
    return matrix;
}

pred_t* getBest(std::vector<float> result, const char* audioFile, pred_t *pred){
    int first, second, third;
    if (result[0] > result[1] && result[0] > result[2]){
        first = 0;
        if (result[1] > result[2]) second = 1, third = 2;
        else second = 2, third = 1;
    }
    else if (result[1] > result[0] && result[1] > result[2]){
        first = 1;
        if (result[0] > result[2]) second = 0, third = 2;
        else second = 2, third = 0;
    }
    else{
        first = 2;
        if (result[0] > result[1]) second = 0, third = 1;
        else second = 1, third = 0;
    }
    for (int i = 3; i < 12; i++){
        if (result[i] > result[first]){
            third = second;
            second = first;
            first = i;
        }
        else if (result[i] > result[second]){
            third = second;
            second = i;
        }
        else if (result[i] > result[third]) third = i;
    }
    pred[0] = pred_t(first, result[first]);
    pred[1] = pred_t(second, result[second]);
    pred[2] = pred_t(third, result[third]);
    return pred;
}

pred_t* libaudioAPI(const char* audioFile, pred_t* pred){
    FILE *file = freopen(audioFile, "r", stdin);
    if (!file) throwInvalidFileError(audioFile);
    // input inputMatrix
    Matrix<float> inputMatrix(1, 250);
    std::cin >> inputMatrix;
    fclose(stdin);
    Matrix<float> WeightMatrix1 = convertRowMajorToMatrix(IP1_WT, 250, 144);
    Matrix<float> WeightMatrix2 = convertRowMajorToMatrix(IP2_WT, 144, 144);
    Matrix<float> WeightMatrix3 = convertRowMajorToMatrix(IP3_WT, 144, 144);
    Matrix<float> WeightMatrix4 = convertRowMajorToMatrix(IP4_WT, 144, 12);
    Matrix<float> ResultMatrix1 = convertRowMajorToMatrix(IP1_BIAS, 1, 144);
    Matrix<float> ResultMatrix2 = convertRowMajorToMatrix(IP2_BIAS, 1, 144);
    Matrix<float> ResultMatrix3 = convertRowMajorToMatrix(IP3_BIAS, 1, 144);
    Matrix<float> ResultMatrix4 = convertRowMajorToMatrix(IP4_BIAS, 1, 12);

    addProductMKL(inputMatrix, WeightMatrix1, ResultMatrix1);
    ResultMatrix1.applyRelu();
    addProductMKL(ResultMatrix1, WeightMatrix2, ResultMatrix2);
    ResultMatrix2.applyRelu();
    addProductMKL(ResultMatrix2, WeightMatrix3, ResultMatrix3);
    ResultMatrix3.applyRelu();
    addProductMKL(ResultMatrix3, WeightMatrix4, ResultMatrix4);
    Vector<float> result(12);
    result.vec = ResultMatrix4.mat[0];
    result.applySoftmax();

    return getBest(result.vec, audioFile, pred);
}   
