#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Matrix.hpp"

using namespace std;

char *inputMatrixFile = "inputMatrix.txt";
char *weightMatrixFile = "weightMatrix.txt";
char *biasMatrixFile = "biasMatrix.txt";

string resultNormal = "";
string resultPthread = "";
string resultMKL = "";
string resultOpenBlas = "";

float random(float l, float r){
    return l + (r - l) * rand() / RAND_MAX;
}

void outputRandomMatrix(int n, char *outputFile){
    Matrix<float> matrix(n, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            matrix.mat[i][j] = random(-10.0f, 10.0f);
        }
    }
    freopen(outputFile, "w", stdout);
    cout << matrix.n << "\n" << matrix.m << "\n";
    cout << matrix << "\n";
    fclose(stdout);
}

void getData(int n, int numTest){
    vector<double> timeNormal(numTest);
    vector<double> timePthread(numTest);
    vector<double> timeMKL(numTest);
    vector<double> timeOpenBlas(numTest);

    for (int i = 0; i < numTest; i++){
        outputRandomMatrix(n, inputMatrixFile);
        outputRandomMatrix(n, weightMatrixFile);
        outputRandomMatrix(n, biasMatrixFile);
        auto start = std::chrono::system_clock::now();
        system("./yourcode.out fullyconnected inputMatrix.txt weightMatrix.txt biasMatrix.txt outputMatrix.txt normal");
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> time_taken = end - start;
        timeNormal[i] = time_taken.count();

        start = std::chrono::system_clock::now();
        system("./yourcode.out fullyconnected inputMatrix.txt weightMatrix.txt biasMatrix.txt outputMatrix.txt pthread");
        end = std::chrono::system_clock::now();
        time_taken = end - start;
        timePthread[i] = time_taken.count();

        start = std::chrono::system_clock::now();
        system("./yourcode.out fullyconnected inputMatrix.txt weightMatrix.txt biasMatrix.txt outputMatrix.txt mkl");
        end = std::chrono::system_clock::now();
        time_taken = end - start;
        timeMKL[i] = time_taken.count();

        start = std::chrono::system_clock::now();
        system("./yourcode.out fullyconnected inputMatrix.txt weightMatrix.txt biasMatrix.txt outputMatrix.txt openblas");
        end = std::chrono::system_clock::now();
        time_taken = end - start;
        timeOpenBlas[i] = time_taken.count();
    }
    double avgTimeNormal = 0.0;
    double avgTimePthread = 0.0;
    double avgTimeMKL = 0.0;
    double avgTimeOpenBlas = 0.0;

    for (int i = 0; i < numTest; i++){
        avgTimeNormal += timeNormal[i];
        avgTimePthread += timePthread[i];
        avgTimeMKL += timeMKL[i];
        avgTimeOpenBlas += timeOpenBlas[i];
    }

    avgTimeNormal /= numTest;
    avgTimePthread /= numTest;
    avgTimeMKL /= numTest;
    avgTimeOpenBlas /= numTest;

    double avgErrorNormal = 0.0;
    double avgErrorPthread = 0.0;
    double avgErrorMKL = 0.0;
    double avgErrorOpenBlas = 0.0;

    for (int i = 0; i < numTest; i++){
        avgErrorNormal += abs(avgTimeNormal - timeNormal[i]);
        avgErrorPthread += abs(avgTimePthread - timePthread[i]);
        avgErrorMKL += abs(avgTimeMKL - timeMKL[i]);
        avgErrorOpenBlas += abs(avgTimeOpenBlas - timeOpenBlas[i]);
    }

    avgErrorNormal /= numTest;
    avgErrorPthread /= numTest;
    avgErrorMKL /= numTest;
    avgErrorOpenBlas /= numTest;

    double stdNormal = 0.0;
    double stdPthread = 0.0;
    double stdMKL = 0.0;
    double stdOpenBlas = 0.0;

    for (int i = 0; i < numTest; i++){
        stdNormal += (avgTimeNormal - timeNormal[i]) * (avgTimeNormal - timeNormal[i]);
        stdPthread += (avgTimePthread - timePthread[i]) * (avgTimePthread - timePthread[i]);
        stdMKL +=  (avgTimeMKL - timeMKL[i]) * (avgTimeMKL - timeMKL[i]);
        stdOpenBlas += (avgTimeOpenBlas - timeOpenBlas[i]) * (avgTimeOpenBlas - timeOpenBlas[i]);
    }

    stdNormal /= numTest; stdNormal = sqrt(stdNormal);
    stdPthread /= numTest; stdPthread = sqrt(stdPthread);
    stdMKL /= numTest; stdMKL = sqrt(stdMKL);
    stdOpenBlas /= numTest; stdOpenBlas = sqrt(stdOpenBlas);


    resultNormal += to_string(n) + " " + to_string(avgTimeNormal) + " " + to_string(avgErrorNormal) + " "  + to_string(stdNormal) + "\n";
    resultPthread += to_string(n) + " " + to_string(avgTimePthread) + " " + to_string(avgErrorPthread) + " " + to_string(stdPthread) + "\n";
    resultMKL += to_string(n) + " " + to_string(avgTimeMKL) + " " + to_string(avgErrorMKL) + " " + to_string(stdMKL) + "\n";
    resultOpenBlas += to_string(n) + " " + to_string(avgTimeOpenBlas) + " " + to_string(avgErrorOpenBlas) + " " + to_string(stdOpenBlas) + "\n";
}

int main(){
    srand(time(0));
    system("make");

    vector<int> to_run = {10, 20, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

    for (int i: to_run){
        cerr << "Running on " << i << "\n";
        getData(i, 10);
    }
    freopen("normalData.txt", "w", stdout);
    cout << resultNormal;
    fclose(stdout);

    freopen("pthreadData.txt", "w", stdout);
    cout << resultPthread;
    fclose(stdout);

    freopen("mklData.txt", "w", stdout);
    cout << resultMKL;
    fclose(stdout);

    freopen("openBlasData.txt", "w", stdout);
    cout << resultOpenBlas;
    fclose(stdout);
    return 0;
}