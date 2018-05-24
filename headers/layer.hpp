#pragma once

#include "neuron.hpp"
#include <vector>

using namespace std;

class HLayer {
    public:
    HLayer(size_t nNeurons);

    virtual ~HLayer() {
        neuron.clear();
    }

    virtual double getSigmo(size_t at) {
        return neuron.at(at).getSigmo();
    };

    double getValue(size_t at);
    double getSigma(size_t at);
    size_t getLenght();

    void setValue(size_t at, double value);
    void setSigma(size_t at, double value);

    vector<double> getOutput();
    protected:
    vector<Neuron> neuron;
};

class ILayer : public HLayer {
    public:
    ILayer(size_t nNeurons) :HLayer(nNeurons) {}
    virtual double getSigmo(size_t at) {
        return neuron.at(at).getValue();
    }
};