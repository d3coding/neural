#include "headers/matrix.hpp"
#include <random>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>

Matrix::Matrix(unsigned int nInput, unsigned int nHLayers, unsigned int nNperHLayers, unsigned int nOutput) :mInput(new ILayer(nInput)), mOutput(new HLayer(nOutput)) {
    for(unsigned int x(0); x < nHLayers; ++x)
        mLayer.push_back(new HLayer(nNperHLayers));
    mLayer.insert(mLayer.begin(), mInput);
    mLayer.insert(mLayer.end(), mOutput);

    genP();
}

Matrix::Matrix(MatrixData *M) :mInput(new ILayer(M->Layer[0])), mOutput(new HLayer(M->Layer[3])) {
    for(unsigned int x(0); x < M->Layer[1]; ++x)
        mLayer.push_back(new HLayer(M->Layer[2]));
    mLayer.insert(mLayer.begin(), mInput);
    mLayer.insert(mLayer.end(), mOutput);

    genP();

    mBias = M->Bias;
    mWeight = M->Weight;
    mData = M->Data;

    for(unsigned int x(0); x < mWeight.size(); ++x) {
        vector<double*> a;
        mDWeight.push_back(a);
        for(unsigned int y(0); y < mWeight.at(x).size(); ++y) {
            mDWeight.back().push_back(new double(0));
        }
    }
    for(unsigned int x(0); x < mBias.size(); ++x) {
        vector<double*> a;
        mDBias.push_back(a);
        for(unsigned int y(0); y < mBias.at(x).size(); ++y) {
            mDBias.back().push_back(new double(0));
        }
    }
    for(unsigned int x(0); x < mData.size(); ++x) {
        vector<double> a;
        mData.at(x).push_back(a);
        for(unsigned int y(0); y < mOutput->getLenght(); ++y)
            mData.at(x).back().push_back(0);
    }
}

Matrix::~Matrix() {
    for(unsigned int x(0); x < mLayer.size(); ++x)
        delete mLayer.at(x);
    for(unsigned int x(0); x < mWeight.size(); ++x) {
        for(unsigned int y(0); y < mWeight.at(x).size(); ++y) {
            delete mWeight.at(x).at(y);
            delete mDWeight.at(x).at(y);
        }
    }
    for(unsigned int x(0); x < mBias.size(); ++x) {
        for(unsigned int y(0); y < mBias.at(x).size(); ++y) {
            delete mBias.at(x).at(y);
            delete mDBias.at(x).at(y);
        }
    }
    mWeight.clear();
    mDWeight.clear();
    mBias.clear();
    mDBias.clear();
    mData.clear();
    mLayer.clear();
}

vector<double> Matrix::getError(unsigned int atData) {
    vector<double> vec;
    for(unsigned int x(0); x < mOutput->getLenght(); ++x)
        vec.push_back(mData.at(atData).at(1).at(x) - mData.at(atData).back().at(x));
    return vec;
}

vector<double> Matrix::calculate(vector<double> dataVec) {
    feedforward(dataVec);
    return mOutput->getOutput();
}

MatrixData* Matrix::getAllData() {
    MatrixData *dat = new MatrixData;
    {
        dat->Layer[0] = mLayer.front()->getLenght();
        dat->Layer[1] = mLayer.size() - 2;
        dat->Layer[2] = mLayer.at(1)->getLenght();
        dat->Layer[3] = mLayer.back()->getLenght();
    }
    dat->rate = mRate;
    dat->Weight = mWeight;
    dat->Bias = mBias;
    dat->Data = mData;
    return dat;
}

vector<double> Matrix::genRand(unsigned int i) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1, 1);
    vector<double> vec;
    for(unsigned int x(0); x < i; ++x)
        vec.push_back(dis(gen));
    return vec;
}

void Matrix::learn(unsigned int iterations) {
    for(unsigned int x(0); x < iterations; ++x) {
        for(unsigned int y(0); y < mData.size(); ++y) {
            feedforward(mData.at(y).at(0));
            sigma(y);
            backpropagation();
            // TESTAR
            mData.at(y).back() = mOutput->getOutput();
        }
    }
}

void Matrix::sigma(unsigned int dataPosition) {
    for(unsigned int y(0); y < mOutput->getLenght(); ++y) {
        mOutput->setSigma(y, (mOutput->getSigmo(y)) * (1 - mOutput->getSigmo(y)) * (mData.at(dataPosition).at(1).at(y) - mOutput->getSigmo(y)));
        mData.at(dataPosition).back().at(y) = mOutput->getSigmo(y);
    }
    for(unsigned int x(mLayer.size() - 2); x > 0; --x) {
        int i(0);
        for(unsigned int y(0); y < x; ++y)
            i += mLayer.at(y)->getLenght();
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            double j(0);
            for(unsigned short z(0); z < mLayer.at(x + 1)->getLenght(); ++z)
                j += mLayer.at(x + 1)->getSigma(z) * *mWeight.at(i + y).at(z);
            mLayer.at(x)->setSigma(y, mLayer.at(x)->getSigmo(y) * (1 - mLayer.at(x)->getSigmo(y)) * j);
        }
    }
}

void Matrix::addData(vector< vector<double> > newData) {
    mData.push_back(newData);
    vector<double> a;
    mData.back().push_back(a);
    for(unsigned int x(0); x < mOutput->getLenght(); ++x)
        mData.back().back().push_back(0);
}

void Matrix::setRate(double value) {
    mRate = value;
}

void Matrix::genP() {
    unsigned int i(0);

    // how may weights
    for(unsigned int x(0); x < mLayer.size() - 1; ++x) {
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            for(unsigned int z(0); z < mLayer.at(x + 1)->getLenght(); ++z) {
                i++;
            }
        }
    }

    // how many bias
    for(unsigned int x(1); x < mLayer.size(); ++x) {
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            i++;
        }
    }

    vector<double> vec = genRand(i);

    for(unsigned int x(0); x < mLayer.size() - 1; ++x) {
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            vector<double*> a;
            vector<double*> b;
            mWeight.push_back(a);
            mDWeight.push_back(b);
            for(unsigned int z(0); z < mLayer.at(x + 1)->getLenght(); ++z) {
                mWeight.back().push_back(new double(vec.at(0)));
                vec.erase(vec.begin());
                mDWeight.back().push_back(new double(0));
            }
        }
    }

    for(unsigned int x(1); x < mLayer.size(); ++x) {
        vector<double*> a;
        vector<double*> b;
        mBias.push_back(a);
        mDBias.push_back(b);
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            mBias.back().push_back(new double(vec.at(0)));
            vec.erase(vec.begin());
            mDBias.back().push_back(new double(0));
        }
    }
}

void Matrix::resetHL() {
    for(unsigned int x(0); x < mLayer.size(); ++x) {
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y)
            mLayer.at(x)->setValue(y, 0);
    }
}

void Matrix::resetP() {
    for(unsigned int x(0); x < mWeight.size(); ++x)
        mWeight.at(x).clear();
    mWeight.clear();
    for(unsigned int x(0); x < mBias.size(); ++x)
        mBias.at(x).clear();
    mBias.clear();
}

void Matrix::feedforward(vector<double> dat) {
    resetHL();
    for(unsigned int x(0); x < mInput->getLenght(); ++x)
        mInput->setValue(x, dat.at(x));
    int i(0), j(0);
    for(unsigned int x(1); x < mLayer.size(); ++x) {
        for(unsigned int y(0); y < mLayer.at(x)->getLenght(); ++y) {
            for(unsigned int z(0); z < mLayer.at(x - 1)->getLenght(); ++z)
                mLayer.at(x)->setValue(y, mLayer.at(x)->getValue(y) + (mLayer.at(x - 1)->getSigmo(z) * *mWeight.at(z + j).at(y)));
            mLayer.at(x)->setValue(y, mLayer.at(x)->getValue(y) - *mBias.at(x - 1).at(y));
            ++i;
        }
        j = i;
    }
}

void Matrix::backpropagation() {
    for(unsigned int atLayer(mLayer.size() - 1); atLayer > 0; --atLayer) {
        for(unsigned int atNeuron(0); atNeuron < mLayer.at(atLayer)->getLenght(); ++atNeuron)
            for(unsigned int x(0); x < mLayer.at(atLayer - 1)->getLenght(); ++x) {
                int i(0);
                for(unsigned int y(0); y < atLayer - 1; ++y)
                    i += mLayer.at(y)->getLenght();
                *mDWeight.at(x + i).at(atNeuron) = (mRate * mLayer.at(atLayer - 1)->getSigmo(x) * mLayer.at(atLayer)->getSigma(atNeuron));
            }
    }
    for(unsigned int x(0); x < mBias.size(); ++x) {
        for(unsigned int y(0); y < mBias.at(x).size(); ++y)
            *mDBias.at(x).at(y) = (mRate * -1 * mLayer.at(x + 1)->getSigma(y));
    }

    for(unsigned int x(0); x < mWeight.size(); ++x) {
        for(unsigned int y(0); y < mWeight.at(x).size(); ++y)
            *mWeight.at(x).at(y) += *mDWeight.at(x).at(y);
    }
    for(unsigned int x(0); x < mBias.size(); ++x) {
        for(unsigned int y(0); y < mBias.at(x).size(); ++y)
            *mBias.at(x).at(y) += *mDBias.at(x).at(y);
    }
}
