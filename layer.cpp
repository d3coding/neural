#include "headers/layer.hpp"
#include <vector>

using namespace std;

Layer::Layer(unsigned int nNeurons) {
  for(unsigned int x(0); x < nNeurons; ++x)
    neuron.push_back(new Neuron);
}

void Layer::setSigma(unsigned int at,double value) {
  neuron.at(at)->setSigma(value);
}

void Layer::setValue(unsigned int at, double value) {
  neuron.at(at)->setValue(value);
}

double Layer::getValue(unsigned int at) {
  return neuron.at(at)->getValue();
}

double Layer::getSigma(unsigned int at) {
  return neuron.at(at)->getSigma();
}

unsigned int Layer::getLenght() {
  return neuron.size();
}

vector<double> Layer::getOutput(){
  vector<double> vec;
  for(unsigned int x(0);x < neuron.size();++x)
    vec.push_back(neuron.at(x)->getSigmo());
  return vec;
}
