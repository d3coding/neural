#include "headers/layer.hpp"
#include <vector>

HLayer::HLayer(size_t nNeurons) {
    for(unsigned int x(0); x < nNeurons; ++x) {
        Neuron mNeuron;
        neuron.push_back(mNeuron);
    }
}

double HLayer::getValue(unsigned int at) {
    return neuron.at(at).getValue();
}

double HLayer::getSigma(unsigned int at) {
    return neuron.at(at).getSigma();
}

size_t HLayer::getLenght() {
    return (unsigned int) neuron.size();
}

void HLayer::setSigma(unsigned int at, double value) {
    neuron.at(at).setSigma(value);
}

void HLayer::setValue(unsigned int at, double value) {
    neuron.at(at).setValue(value);
}

vector<double> HLayer::getOutput() {
    vector<double> vec;
    for(unsigned int x(0); x < neuron.size(); ++x)
        vec.push_back(neuron.at(x).getSigmo());
    return vec;
}
