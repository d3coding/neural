#pragma once

#include "layer.hpp"
#include <iostream>
#include <vector>

using namespace std;

struct MatrixData {

    size_t nInput, nHLayers, nNperHLayers, nOutput;
    double rate;
    vector< vector<double> > Weight;
    vector< vector<double> > Bias;
    vector< vector< vector<double> > > Data;
};

class Matrix {
    public:
    Matrix(size_t nInput, size_t nHLayers, size_t nNperHLayers, size_t nOutput, bool gen = true);
    ~Matrix();

    static Matrix* useMatrixData(MatrixData mMatrix);

    vector<double> getError(unsigned int atData);
    vector<double> calculate(vector<double> input);
    MatrixData* getAllData();
    vector<double> genRand(unsigned int i);

    void sigma(unsigned int dataPosition);
    void addData(vector< vector<double> > newData);
    void setRate(double newRate);

    void learnFor(unsigned int iterations);

    void genP(unsigned int totalParam);
    void resetHL();
    void resetP();

    void feedforward(vector<double> dat);
    void backpropagation();

    int setWeight(vector< vector<double> > newWeight);
    int setBias(vector< vector<double> > newBias);
    int setData(vector< vector< vector<double> > > newData);

    unsigned int getWn();
    unsigned int getBn();
    double getRate();

    private:
    vector< vector<double> > mWeight;
    vector< vector<double> > mDWeight;
    vector< vector<double> > mBias;
    vector< vector<double> > mDBias;
    vector< vector< vector<double> > > mData;
    vector<HLayer*> mLayer;
    double mRate;
};
