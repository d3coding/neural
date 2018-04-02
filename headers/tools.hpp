#pragma once

#include "matrix.hpp"
#include <thread>

using namespace std;

class Tools {
    public:
    Tools(unsigned int a, unsigned int b, unsigned int c, unsigned int d) : m(new Matrix(a, b, c, d)) {}
    Tools(Matrix *M) :m(M) {}
    ~Tools();

    static Matrix* useLogicGate(double ff, double ft, double tf, double tt);

    void learn(unsigned int iterations);
    void logicCalc(bool logic = false);
    void printResult();

    void setMatrix(Matrix *newMatrix);

    Matrix *getMatrix();

    static void final();

    private:
    Matrix * m;
};
