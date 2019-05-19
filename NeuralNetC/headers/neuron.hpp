#pragma once

#include <cmath>

class Neuron {
    public:
    Neuron() :value(0), S(0) {}
    void setValue(double x) {
        value = x;
    }
    void setSigma(double x) {
        S = x;
    }
    double getSigmo() {
        return (1 / (1 + exp(-value)));
    }
    double getValue() {
        return value;
    }
    double getSigma() {
        return S;
    }
    private:
    double value, S;
};

