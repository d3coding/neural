#ifndef MATRIX_H
#define MATRIX_H

#include "layer.hpp"
#include <iostream>
#include <vector>

using namespace std;

struct MatrixData{
    vector<unsigned int> Layer;
    vector< vector<double*> > Weight;
    vector< vector<double*> > Bias;
    vector< vector< vector<double> > > Data;
    string presetName;
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
    void defRate(double newRate);

    void learn(unsigned int iterations);
    void stop();

    void genP();
    void resetHL();
    void resetP();

    void feedforward(vector<double> dat);
    void backpropagation();

  private:
    vector< vector<double*> > weight;
    vector< vector<double*> > dWeight;
    vector< vector<double*> > bias;
    vector< vector<double*> > dBias;
    vector< vector< vector<double> > > data;
    vector<Layer*> layer;
    unsigned int *leftIterations;
    ILayer *Input;
    HLayer *Output;
    double rate;
};

#endif // MATRIX_H
