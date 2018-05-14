#include "headers/layer.hpp"
#include <vector>

Layer::Layer(unsigned int nNeurons) {
    for(unsigned int x(0); x < nNeurons; ++x)
        neuron.push_back(new Neuron);
}

double Layer::getValue(unsigned int at) {
    return neuron.at(at)->getValue();
}

double Layer::getSigma(unsigned int at) {
    return neuron.at(at)->getSigma();
}

unsigned int Layer::getLenght() {
    return (unsigned int) neuron.size();
}

void Layer::setSigma(unsigned int at, double value) {
    neuron.at(at)->setSigma(value);
}

void Layer::setValue(unsigned int at, double value) {
    neuron.at(at)->setValue(value);
}

vector<double> Layer::getOutput() {
    vector<double> vec;
    for(unsigned int x(0); x < neuron.size(); ++x)
        vec.push_back(neuron.at(x)->getSigmo());
    return vec;
}
