#pragma once

#include "matrix.hpp"
#include <thread>

using namespace std;

class Tools {
    public:
    Tools(size_t a, size_t b, size_t c, size_t d) : m(new Matrix(a, b, c, d)) {}
    Tools(Matrix *M) :m(M) {}
    ~Tools();

    static Matrix* useLogicGate(double ff, double ft, double tf, double tt);

    int learn(unsigned int iterations);
    void logicCalc(bool logic = false);
    void printResult();

    void setMatrix(Matrix *newMatrix);

    Matrix getMatrix();

    static void final();

    private:
    Matrix * m;
};
