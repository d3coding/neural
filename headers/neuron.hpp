#ifndef NEURON_H
#define NEURON_H

#include <cmath>

class Neuron {
  public:
    Neuron():value(0),S(0){}
    ~Neuron(){
    //  delete value;
  //    delete S;
    }
    void setValue(double x) {
      value = x;
    }
    void setSigma(double x) {
      S = x;
    }
    double getSigmo() {
      return (1/(1+exp(-getValue())));
    }
    double getValue() {
      return value;
    }
    double getSigma() {
      return S;
    }
  private:
    double value,S;
};

#endif // NEURON_H
