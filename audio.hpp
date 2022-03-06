#ifndef AUDIO_BY_TANMAY_SARTHAK
#define AUDIO_BY_TANMAY_SARTHAK

#include<iostream>
#include"Vector.hpp"
#include"Matrix.hpp"
#include"MKL.hpp"
#include"dnn_weights.h"

// Pred_t structure
struct pred_t{
    int label;
    float prob;
    pred_t(){}
    pred_t(int label, float prob){
        this->label = label;
        this->prob = prob;
    }
};

extern pred_t* libaudioAPI(const char* audioFile, pred_t* pred);

#endif