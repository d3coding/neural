#pragma once

#include "neuron.hpp"
#include <vector>

using namespace std;

class Layer {
    public:
    Layer(unsigned int nNeurons);

    virtual ~Layer() {
        for(unsigned int x(0); x < neuron.size(); ++x)
            delete neuron.at(x);
        neuron.clear();
    }

    virtual double getSigmo(unsigned int at) = 0;

    vector<double> getOutput();
    double getValue(unsigned int at);
    double getSigma(unsigned int at);
    unsigned int getLenght();
    void setSigma(unsigned int at, double value);
    void setValue(unsigned int at, double value);
    protected:
    vector<Neuron*> neuron;
};

class ILayer : public Layer {
    public:
    ILayer(unsigned int nNeurons) :Layer(nNeurons) {}
    virtual double getSigmo(unsigned int at) {
        return neuron.at(at)->getValue();
    }
};

class HLayer : public Layer {
    public:
    HLayer(unsigned int nNeurons) :Layer(nNeurons) {}
    virtual double getSigmo(unsigned int at) {
        return neuron.at(at)->getSigmo();
    }
};
