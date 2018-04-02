#include "headers/tools.hpp"
#include "headers/matrix.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <chrono>

Tools::~Tools() {
    if(m != NULL)
        delete m;
}

Matrix* Tools::useLogicGate(double ff, double ft, double tf, double tt) {
    Matrix *M = new Matrix(2, 1, 2, 1);
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
    for(unsigned int x(0); x < data.size(); ++x)
        M->addData(data.at(x));
    return M;
}

void Tools::learn(unsigned int iterations) {
    if(iterations == 0) {
        cout << "# ERR # AS ITERACOES SAO 0!\n" << endl;
        exit(1);
    } else {
        auto start = chrono::system_clock::now();
        cout << "# Treinando...\n";
        m->learn(iterations);
        auto end = chrono::system_clock::now();
        cout << "# Sucesso ao treinar!...\n" << "# Tempo decorrido: " << (chrono::duration_cast<chrono::milliseconds>(end - start)).count() << "ms\n";
    }
}

void Tools::logicCalc(bool logic) {
    vector<double> fInput;
    MatrixData *data = m->getAllData();
    for(unsigned int x(0); x < data->Layer[0]; ++x) {
        cout << "# Digite o valor de entrada para o neuronio(" << x << "):";
        double y;
        cin >> y;
        fInput.push_back(y);
    }
    delete data;
    vector<double> result = m->calculate(fInput);
    cout << "# Mostrando resultados:\n";
    if(logic) {
        for(unsigned int x(0); x < result.size(); ++x) {
            cout << "# \tR" << x << ": " << (!(result.at(x) < .5) ? "Verdadeiro" : "Falso") << endl;
        }
    } else {
        for(unsigned int x(0); x < result.size(); ++x) {
            cout << "# \tR" << x << ": " << result.at(x) << endl;
        }
    }
}

void Tools::printResult() {
    MatrixData *tmp = m->getAllData();
    cout << "########## Weight ###########\n";
    for(unsigned int x(0); x < tmp->Weight.size(); ++x) {
        for(unsigned int y(0); y < tmp->Weight.at(x).size(); ++y)
            cout << "[" << x << "," << y << "] " << *tmp->Weight.at(x).at(y) << "\t";
        cout << endl;
    }
    cout << "\n########## Bias ##########\n";
    for(unsigned int x(0); x < tmp->Bias.size(); ++x) {
        for(unsigned int y(0); y < tmp->Bias.at(x).size(); ++y)
            cout << "[" << x << "," << y << "] " << *tmp->Bias.at(x).at(y) << "\t";
        cout << "\n";
    }
    cout << "\n########## Experado ##########\n";
    for(unsigned int x(0); x < tmp->Data.size(); ++x) {
        for(unsigned int y(0); y < tmp->Layer[3]; ++y)
            cout << "EXPER(" << x << "):\t" << tmp->Data.at(x).at(1).at(y) << "\t";
        cout << "\n";
        for(unsigned int y(0); y < tmp->Layer[3]; ++y)
            cout << "ERRO(" << x << "):\t" << tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y) << "\t";
        cout << "\n";
    }
    cout << "\n########## Erro ##########\n";
    double tot(0.0);
    int n(0);
    for(unsigned int x(0); x < tmp->Data.size(); ++x) {
        for(unsigned int y(0); y < tmp->Layer[3]; ++y) {
            tot += abs(tmp->Data.at(x).at(1).at(y) - tmp->Data.at(x).back().at(y));
            ++n;
        }
    }
    cout << "Total:\t\t" << tot << endl;
    cout << "Porcentagem:\t" << (100 * tot) / n << "%\n";
}

void Tools::setMatrix(Matrix *newMatrix) {
    m = newMatrix;
}

Matrix* Tools::getMatrix() {
    return m;
}

void Tools::final() {
    cout << "\n# Aperte enter para continuar...\n";
    getchar();
}