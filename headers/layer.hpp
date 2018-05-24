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

    virtual double getSigmo(unsigned int at) {
        return neuron.at(at).getSigmo();
    };

    double getValue(unsigned int at);
    double getSigma(unsigned int at);
    size_t getLenght();

    void setValue(unsigned int at, double value);
    void setSigma(unsigned int at, double value);

    vector<double> getOutput();
    protected:
    vector<Neuron> neuron;
};

class ILayer : public HLayer {
    public:
    ILayer(unsigned int nNeurons) :HLayer(nNeurons) {}
    virtual double getSigmo(unsigned int at) {
        return neuron.at(at).getValue();
    }
};