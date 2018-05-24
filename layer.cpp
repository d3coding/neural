#include "headers/layer.hpp"
#include <vector>

HLayer::HLayer(size_t nNeurons) {
    for(unsigned int x(0); x < nNeurons; ++x) {
        Neuron mNeuron;
        neuron.push_back(mNeuron);
    }
}

double HLayer::getValue(size_t at) {
    return neuron.at(at).getValue();
}

double HLayer::getSigma(size_t at) {
    return neuron.at(at).getSigma();
}

size_t HLayer::getLenght() {
    return (unsigned int) neuron.size();
}

void HLayer::setSigma(size_t at, double value) {
    neuron.at(at).setSigma(value);
}

void HLayer::setValue(size_t at, double value) {
    neuron.at(at).setValue(value);
}

vector<double> HLayer::getOutput() {
    vector<double> vec;
    for(unsigned int x(0); x < neuron.size(); ++x)
        vec.push_back(neuron.at(x).getSigmo());
    return vec;
}
