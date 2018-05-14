#pragma once

#include "layer.hpp"
#include <iostream>
#include <vector>

using namespace std;

struct MatrixData {

    unsigned int nInput, nHLayers, nNperHLayers, nOutput;
    double rate;
    vector< vector<double> > Weight;
    vector< vector<double> > Bias;
    vector< vector< vector<double> > > Data;
};

class Matrix {
    public:
    Matrix(unsigned int nInput, unsigned int nHLayers, unsigned int nNperHLayers, unsigned int nOutput);
    Matrix(MatrixData *dt);
    ~Matrix();

    vector<double> getError(unsigned int atData);
    vector<double> calculate(vector<double> input);
    MatrixData* getAllData();
    vector<double> genRand(unsigned int i);

    void sigma(unsigned int dataPosition);
    void addData(vector< vector<double> > newData);
    void setRate(double newRate);

    void learnFor(unsigned int iterations);

    void genP();
    void resetHL();
    void resetP();

    void feedforward(vector<double> dat);
    void backpropagation();

    private:
    vector< vector<double> > mWeight;
    vector< vector<double> > mDWeight;
    vector< vector<double> > mBias;
    vector< vector<double> > mDBias;
    vector< vector< vector<double> > > mData;
    vector<HLayer*> mLayer;
    double mRate;
};
