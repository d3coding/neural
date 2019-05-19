#include "headers/tools.hpp"
#include "headers/matrix.hpp"
#include "headers/strings.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iomanip>

Tools::~Tools() {
    if(m != NULL)
        delete m;
}

Matrix* Tools::useLogicGate(double ff, double ft, double tf, double tt) {
    Matrix *M = new Matrix(2, 1, 2, 1);
    M->setRate(0.05);
    vector< vector< vector<double> > > data;
    for(int x(0); x < 4; ++x) {
        vector< vector<double> > a;
        data.push_back(a);
        for(unsigned int y(0); y < 2; ++y) {
            vector<double> a;
            data.at(x).push_back(a);
        }
    }
    {
        {
            data.at(0).at(0).push_back(0);
            data.at(0).at(0).push_back(0);
            data.at(0).at(1).push_back(ff);
        }
        {
            data.at(1).at(0).push_back(0);
            data.at(1).at(0).push_back(1);
            data.at(1).at(1).push_back(ft);
        }
        {
            data.at(2).at(0).push_back(1);
            data.at(2).at(0).push_back(0);
            data.at(2).at(1).push_back(tf);
        }
        {
            data.at(3).at(0).push_back(1);
            data.at(3).at(0).push_back(1);
            data.at(3).at(1).push_back(tt);
        }
    }
    for(size_t x = 0; x < data.size(); ++x)
        M->addData(data.at(x));
    return M;
}

int Tools::learn(unsigned int iterations) {
    if(iterations == 0) {
        cout << ERR1c00 << endl;
        return -1;
    } else if(m->getRate() <= 0) {
        cout << ERR1c01 << endl;
        return -1;
    } else {
        auto start = chrono::system_clock::now();
		cout << TX1c00;
        m->learnFor(iterations);
        auto end = chrono::system_clock::now();
        cout << TX1c01 << (chrono::duration_cast<chrono::milliseconds>(end - start)).count() << "ms\n";
    }
    return 0;
}

void Tools::logicCalc(bool logic) {
    vector<double> fInput;
    MatrixData *data = m->getAllData();
    for(unsigned int x = 0; x < data->nInput; ++x) {
        cout << TX1c02 << x << "):";
        double y;
        cin >> y;
        fInput.push_back(y);
    }
    delete data;
    vector<double> result = m->calculate(fInput);
    cout << TX1c03;
    if(logic) {
        for(size_t x = 0; x < result.size(); ++x) {
            cout << "# \tR" << x << ": " << (!(result.at(x) < .5) ? TX1c04 : TX1c05) << endl;
        }
    } else {
        for(size_t x = 0; x < result.size(); ++x) {
            cout << "# \tR" << x << ": " << result.at(x) << endl;
        }
    }
}

void Tools::printResult() {
    MatrixData *tmp = m->getAllData();
    cout << TX1c10;
    for(size_t x = 0; x < tmp->Weight.size(); ++x) {
        for(size_t y = 0; y < tmp->Weight.at(x).size(); ++y)
            cout << "[" << x << "," << y << "] " << tmp->Weight.at(x).at(y) << "\t";
        cout << endl;
    }
    cout << TX1c11;
    for(size_t x = 0; x < tmp->Bias.size(); ++x) {
        for(size_t y = 0; y < tmp->Bias.at(x).size(); ++y)
            cout << "[" << x << "," << y << "] " << tmp->Bias.at(x).at(y) << "\t";
        cout << "\n";
    }
    cout << TX1c12;
    for(size_t x = 0; x < tmp->Data.size(); ++x) {
        for(size_t y(0); y < tmp->nOutput; ++y)
            cout << TX1c16 << x << "):\t" << tmp->Data.at(x).at(1).at(y) << "\t";
        cout << "\n";
        for(size_t y = 0; y < tmp->nOutput; ++y)
            cout << TX1c17 << x << "):\t" << abs(tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y)) << "\t";
        cout << "\n";
    }
    cout << TX1c13;
    double tot(0.0);
    int n(0);
    for(size_t x(0); x < tmp->Data.size(); ++x) {
        for(size_t y(0); y < tmp->nOutput; ++y) {
            tot += abs(tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y));
            ++n;
        }
    }
    streamsize dflt = cout.precision();
    cout << TX1c14 << tot << endl;
    cout << setprecision(2) << TX1c15 << (100 * tot) / n << "%\n" << setprecision(dflt);
}

void Tools::setMatrix(Matrix *newMatrix) {
    m = newMatrix;
}

Matrix Tools::getMatrix() {
    return *m;
}

void Tools::final() {
	cout << TX1c09;
    getchar();
}
