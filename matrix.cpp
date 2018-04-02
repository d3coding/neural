#include "headers/matrix.hpp"
#include <random>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

void Matrix::defRate(double newRate){
	rate = newRate;
}

Matrix::Matrix(unsigned int nInput,unsigned int nHLayers,unsigned int nNperHLayers,unsigned int nOutput):Input(new ILayer(nInput)),Output(new HLayer(nOutput)){
  for(unsigned int x(0); x < nHLayers; ++x)
    layer.push_back(new HLayer(nNperHLayers));
  layer.insert(layer.begin(),Input);
  layer.insert(layer.end(),Output);

  genP();
}

Matrix::Matrix(MatrixData *dt):Input(new ILayer(dt->Layer.front())),Output(new HLayer(dt->Layer.back())){
  for(unsigned int x(0); x < dt->Layer.size()-2; ++x)
    layer.push_back(new HLayer(dt->Layer.at(x+1)));
  layer.insert(layer.begin(),Input);
  layer.insert(layer.end(),Output);

  bias = dt->Bias;
  weight = dt->Weight;
  data = dt->Data;

  for(unsigned int x(0);x < weight.size();++x){
    vector<double*> a;
    dWeight.push_back(a);
    for(unsigned int y(0);y < weight.at(x).size();++y){
      dWeight.back().push_back(new double(0));
    }
  }
  for(unsigned int x(0);x < bias.size();++x){
    vector<double*> a;
    dBias.push_back(a);
    for(unsigned int y(0);y < bias.at(x).size();++y){
      dBias.back().push_back(new double(0));
    }
  }
  for(unsigned int x(0);x < data.size();++x){
    vector<double> a;
    data.at(x).push_back(a);
    for(unsigned int y(0);y < Output->getLenght();++y)
      data.at(x).back().push_back(0);
  }
}

Matrix::~Matrix(){
  for(unsigned int x(0);x < layer.size();++x)
    delete layer.at(x);
  for(unsigned int x(0);x < weight.size();++x){
    for(unsigned int y(0);y < weight.at(x).size();++y){
      delete weight.at(x).at(y);
      delete dWeight.at(x).at(y);
    }
  }
  for(unsigned int x(0);x < bias.size();++x){
    for(unsigned int y(0);y < bias.at(x).size();++y){
      delete bias.at(x).at(y);
      delete dBias.at(x).at(y);
    }
  }
  weight.clear();
  dWeight.clear();
  bias.clear();
  dBias.clear();
  data.clear();
  layer.clear();
}

MatrixData* Matrix::getAllData(){
  MatrixData *dat = new MatrixData;
  for(unsigned int x(0);x < layer.size();++x)
    dat->Layer.push_back(layer.at(x)->getLenght());
  dat->Weight = weight;
  dat->Bias = bias;
  dat->Data = data;
  return dat;
}

vector<double> Matrix::genRand(unsigned int i){
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(-1,1);
  vector<double> vec;
  for(unsigned int x(0);x < i;++x)
    vec.push_back(dis(gen));
  return vec;
}

void Matrix::genP() {
  unsigned int i(0);

  // how may weights
  for(unsigned int x(0);x < layer.size()-1;++x){
    for(unsigned int y(0);y < layer.at(x)->getLenght();++y){
      for(unsigned int z(0);z < layer.at(x+1)->getLenght();++z){
        i++;
      }
    }
  }

  // how many bias
  for(unsigned int x(1);x < layer.size();++x){
    for(unsigned int y(0);y < layer.at(x)->getLenght();++y){
      i++;
    }
  }

  vector<double> vec = genRand(i);

  for(unsigned int x(0);x < layer.size()-1;++x){
    for(unsigned int y(0);y < layer.at(x)->getLenght();++y){
      vector<double*> a;
      vector<double*> b;
      weight.push_back(a);
      dWeight.push_back(b);
      for(unsigned int z(0);z < layer.at(x+1)->getLenght();++z){
        weight.back().push_back(new double(vec.at(0)));
        vec.erase(vec.begin());
        dWeight.back().push_back(new double(0));
      }
    }
  }

  for(unsigned int x(1); x < layer.size(); ++x) {
    vector<double*> a;
    vector<double*> b;
    bias.push_back(a);
    dBias.push_back(b);
    for(unsigned int y(0); y < layer.at(x)->getLenght(); ++y){
      bias.back().push_back(new double(vec.at(0)));
      vec.erase(vec.begin());
      dBias.back().push_back(new double(0));
    }
  }
}

vector<double> Matrix::calculate(vector<double> dataVec){
  feedforward(dataVec);
  return Output->getOutput();
}

vector<double> Matrix::getError(unsigned int atData){
  vector<double> vec;
  for(unsigned int x(0);x < Output->getLenght();++x)
    vec.push_back(data.at(atData).at(1).at(x) - data.at(atData).back().at(x));
  return vec;
}

#ifdef __linux
  void Matrix::learn(unsigned int iterations) {
    leftIterations = &iterations;
    for(unsigned int *x(&iterations);*x;--*x) {
      for(unsigned int y(0);y < data.size(); ++y) {
        feedforward(data.at(y).at(0));
        sigma(y);
        backpropagation();
        data.at(y).back() = Output->getOutput();
      }
    }
  }

  void Matrix::stop(){
    *leftIterations = 5;
  }

#else
  void Matrix::learn(unsigned int iterations){
    for(unsigned int x(0);x < iterations;++x) {
      for(unsigned int y(0);y < data.size(); ++y) {
        feedforward(data.at(y).at(0));
        sigma(y);
        backpropagation();
        data.at(y).back() = Output->getOutput();
      }
    }
  }
#endif // __linux

void Matrix::addData(vector< vector<double> > newData) {
  data.push_back(newData);
  vector<double> a;
  data.back().push_back(a);
  for(unsigned int x(0);x < Output->getLenght();++x)
    data.back().back().push_back(0);
}

void Matrix::resetHL() {
  for(unsigned int x(0); x < layer.size(); ++x) {
    for(unsigned int y(0); y < layer.at(x)->getLenght(); ++y)
      layer.at(x)->setValue(y,0);
  }
}

void Matrix::feedforward(vector<double> dat) {
  resetHL();
  for(unsigned int x(0); x < Input->getLenght(); ++x)
    Input->setValue(x,dat.at(x));
  int i(0),j(0);
  for(unsigned int x(1); x < layer.size(); ++x) {
    for(unsigned int y(0); y < layer.at(x)->getLenght(); ++y) {
      for(unsigned int z(0); z < layer.at(x-1)->getLenght(); ++z)
        layer.at(x)->setValue(y,layer.at(x)->getValue(y) + (layer.at(x-1)->getSigmo(z) * *weight.at(z+j).at(y)));
      layer.at(x)->setValue(y,layer.at(x)->getValue(y) - *bias.at(x-1).at(y));
      ++i;
    }
    j = i;
  }
}

void Matrix::sigma(unsigned int dataPosition) {
  for(unsigned int y(0); y < Output->getLenght(); ++y){
    Output->setSigma(y,(Output->getSigmo(y)) * (1-Output->getSigmo(y)) * (data.at(dataPosition).at(1).at(y) - Output->getSigmo(y)));
    data.at(dataPosition).back().at(y) = Output->getSigmo(y);
  }
  for(unsigned int x(layer.size()-2);x > 0;--x){
    int i(0);
    for(unsigned int y(0); y < x; ++y)
      i += layer.at(y)->getLenght();
    for(unsigned int y(0); y < layer.at(x)->getLenght(); ++y) {
      double j(0);
      for(unsigned short z(0);z < layer.at(x+1)->getLenght() ;++z)
        j += layer.at(x+1)->getSigma(z) * *weight.at(i+y).at(z);
      layer.at(x)->setSigma(y,layer.at(x)->getSigmo(y) * (1 - layer.at(x)->getSigmo(y)) * j);
    }
  }
}

void Matrix::resetP(){
  for(unsigned int x(0);x < weight.size();++x)
    weight.at(x).clear();
  weight.clear();
  for(unsigned int x(0);x < bias.size();++x)
    bias.at(x).clear();
  bias.clear();
}

void Matrix::backpropagation() {
  for(unsigned int atLayer(layer.size()-1); atLayer > 0; --atLayer) {
    for(unsigned int atNeuron(0); atNeuron < layer.at(atLayer)->getLenght(); ++atNeuron)
      for(unsigned int x(0); x < layer.at(atLayer-1)->getLenght(); ++x) {
        int i(0);
        for(unsigned int y(0); y < atLayer-1; ++y)
          i += layer.at(y)->getLenght();
        *dWeight.at(x+i).at(atNeuron) = (rate * layer.at(atLayer-1)->getSigmo(x) * layer.at(atLayer)->getSigma(atNeuron));
      }
  }
  for(unsigned int x(0); x < bias.size(); ++x) {
    for(unsigned int y(0); y < bias.at(x).size(); ++y)
      *dBias.at(x).at(y) = (rate * -1 * layer.at(x+1)->getSigma(y));
  }

  for(unsigned int x(0); x < weight.size(); ++x) {
    for(unsigned int y(0); y < weight.at(x).size(); ++y)
      *weight.at(x).at(y) += *dWeight.at(x).at(y);
  }
  for(unsigned int x(0); x < bias.size(); ++x) {
    for(unsigned int y(0); y < bias.at(x).size(); ++y)
      *bias.at(x).at(y) += *dBias.at(x).at(y);
  }
}
