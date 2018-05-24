#include "headers/matrix.hpp"
#include <random>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>

Matrix::Matrix(size_t nInput, size_t nHLayers, size_t nNperHLayers, size_t nOutput, bool gen) {
    for(size_t x = 0; x < nHLayers; ++x)
        mLayer.push_back(new HLayer(nNperHLayers));
    mLayer.insert(mLayer.begin(),new ILayer(nInput));
    mLayer.insert(mLayer.end(), new HLayer(nOutput));

    if(gen) genP(getWn() + getBn());
}

Matrix* Matrix::useMatrixData(MatrixData mMatrix) {
    cout << "Criando matrix atraves de dados..." << endl;
    Matrix *m = new Matrix(mMatrix.nInput, mMatrix.nHLayers, mMatrix.nNperHLayers, mMatrix.nOutput, false);

    if(m->setBias(mMatrix.Bias) < 0) {
        cout << "# ERR # Error (Bias)" << endl;
        delete m;
        return NULL;
    } else if(m->setWeight(mMatrix.Weight) < 0) {
        cout << "# ERR # Error (Weight)" << endl;
        delete m;
        return NULL;
    } else if(m->setData(mMatrix.Data) < 0) {
        cout << "# ERR # Error (Data)" << endl;
        delete m;
        return NULL;
    } else return m;
}

int Matrix::setWeight(vector< vector<double> > newWeight) {
    cout << "# # Adicionando weight..." << endl;
    if(newWeight.size() != getWn())
        return -1;

    mWeight = newWeight;

    for(size_t x = 0; x < mWeight.size(); ++x) {
        vector<double> a;
        mDWeight.push_back(a);
        for(size_t y = 0; y < mWeight.at(x).size(); ++y)
            mDWeight.back().push_back(0);
    }

    return 0;
}

int Matrix::setBias(vector< vector<double> > newBias) {
    cout << "# # Adicionando bias..." << endl;
    if(newBias.size() != getBn())
        return -1;

    mBias = newBias;

    for(size_t x = 0; x < mBias.size(); ++x) {
        vector<double> a;
        mDBias.push_back(a);
        for(size_t y = 0; y < mBias.at(x).size(); ++y)
            mDBias.back().push_back(0);
    }

    return 0;
}

int Matrix::setData(vector< vector< vector<double> > > newData) {
    return 0;
}

double Matrix::getRate() {
    return mRate;
}

Matrix::~Matrix() {
    for(unsigned int x(0); x < mLayer.size(); ++x)
        delete mLayer.at(x);
    mWeight.clear();
    mDWeight.clear();
    mBias.clear();
    mDBias.clear();
    mData.clear();
    mLayer.clear();
}

vector<double> Matrix::getError(size_t atData) {
    vector<double> vec;
    for(size_t x = 0; x < mLayer.back()->getLenght(); ++x)
        vec.push_back(mData.at(atData).at(1).at(x) - mData.at(atData).back().at(x));
    return vec;
}

vector<double> Matrix::calculate(vector<double> dataVec) {
    feedforward(dataVec);
    return mLayer.back()->getOutput();
}

MatrixData* Matrix::getAllData() {
    MatrixData *dat = new MatrixData;
    {
        dat->nInput = mLayer.front()->getLenght();
        dat->nHLayers = (unsigned int) (mLayer.size() - 2);
        dat->nNperHLayers = mLayer.at(1)->getLenght();
        dat->nOutput = mLayer.back()->getLenght();
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

void Matrix::learnFor(unsigned int iterations) {
    for(unsigned int x = 0; x < iterations; ++x) {
        for(unsigned int y = 0; y < mData.size(); ++y) {
            feedforward(mData.at(y).at(0));
            sigma(y);
            backpropagation();
        }
    }
}

void Matrix::sigma(size_t dataPosition) {
    for(size_t y = 0; y < mLayer.back()->getLenght(); ++y) {
        mLayer.back()->setSigma(y, (mLayer.back()->getSigmo(y)) * (1 - mLayer.back()->getSigmo(y)) * (mData.at(dataPosition).at(1).at(y) - mLayer.back()->getSigmo(y)));
        mData.at(dataPosition).back().at(y) = mLayer.back()->getSigmo(y);
    }
    for(size_t x(mLayer.size() - 2); x > 0; --x) {
        size_t i = 0;
        for(unsigned int y(0); y < x; ++y)
            i += mLayer.at(y)->getLenght();
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            double j(0);
            for(unsigned short z(0); z < mLayer.at(x + 1)->getLenght(); ++z)
                j += mLayer.at(x + 1)->getSigma(z) * mWeight.at(i + y).at(z);
            mLayer.at(x)->setSigma(y, mLayer.at(x)->getSigmo(y) * (1 - mLayer.at(x)->getSigmo(y)) * j);
        }
    }
}

void Matrix::addData(vector< vector<double> > newData) {
    mData.push_back(newData);
    vector<double> a;
    mData.back().push_back(a);
    for(size_t x = 0; x < mLayer.back()->getLenght(); ++x)
        mData.back().back().push_back(0);
}

void Matrix::setRate(double value) {
    mRate = value;
}

unsigned int Matrix::getWn() {
    unsigned int i = 0;
    for(size_t x = 0; x < mLayer.size() - 1; ++x) {
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            for(size_t z = 0; z < mLayer.at(x + 1)->getLenght(); ++z) {
                i++;
            }
        }
    }
    return i;
}

unsigned int Matrix::getBn() {
    unsigned int i = 0;
    for(size_t x = 1; x < mLayer.size(); ++x) {
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            i++;
        }
    }
    return i;
}

void Matrix::genP(unsigned int totalParam) {
    vector<double> vec = genRand(totalParam);

    for(size_t x = 0; x < mLayer.size() - 1; ++x) {
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            vector<double> a;
            vector<double> b;
            mWeight.push_back(a);
            mDWeight.push_back(b);
            for(unsigned int z(0); z < mLayer.at(x + 1)->getLenght(); ++z) {
                mWeight.back().push_back(vec.at(0));
                vec.erase(vec.begin());
                mDWeight.back().push_back(0);
            }
        }
    }

    for(size_t x = 1; x < mLayer.size(); ++x) {
        vector<double> a;
        vector<double> b;
        mBias.push_back(a);
        mDBias.push_back(b);
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            mBias.back().push_back(vec.at(0));
            vec.erase(vec.begin());
            mDBias.back().push_back(0);
        }
    }
}

void Matrix::resetHL() {
    for(size_t x = 0; x < mLayer.size(); ++x) {
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y)
            mLayer.at(x)->setValue(y, 0);
    }
}

void Matrix::resetP() {
    for(size_t x = 0; x < mWeight.size(); ++x)
        mWeight.at(x).clear();
    mWeight.clear();
    for(size_t x = 0; x < mBias.size(); ++x)
        mBias.at(x).clear();
    mBias.clear();
}

void Matrix::feedforward(vector<double> dat) {
    resetHL();
    for(size_t x = 0; x < mLayer.front()->getLenght(); ++x)
        mLayer.front()->setValue(x, dat.at(x));
    unsigned int i = 0, j = 0;
    for(size_t x = 1; x < mLayer.size(); ++x) {
        for(size_t y = 0; y < mLayer.at(x)->getLenght(); ++y) {
            for(size_t z = 0; z < mLayer.at(x - 1)->getLenght(); ++z)
                mLayer.at(x)->setValue(y, mLayer.at(x)->getValue(y) + (mLayer.at(x - 1)->getSigmo(z) * mWeight.at(z + j).at(y)));
            mLayer.at(x)->setValue(y, mLayer.at(x)->getValue(y) - mBias.at(x - 1).at(y));
            ++i;
        }
        j = i;
    }
}

void Matrix::backpropagation() {
    for(size_t atLayer(mLayer.size() - 1); atLayer > 0; --atLayer) {
        for(size_t atNeuron = 0; atNeuron < mLayer.at(atLayer)->getLenght(); ++atNeuron)
            for(size_t x = 0; x < mLayer.at(atLayer - 1)->getLenght(); ++x) {
                size_t i = 0;
                for(size_t y = 0; y < atLayer-1; ++y)
                    i += mLayer.at(y)->getLenght();
                mDWeight.at(x + i).at(atNeuron) = (mRate * mLayer.at(atLayer - 1)->getSigmo(x) * mLayer.at(atLayer)->getSigma(atNeuron));
            }
    }
    for(size_t x = 0; x < mBias.size(); ++x) {
        for(size_t y(0); y < mBias.at(x).size(); ++y)
            mDBias.at(x).at(y) = (mRate * -1 * mLayer.at(x + 1)->getSigma(y));
    }

    for(size_t x = 0; x < mWeight.size(); ++x) {
        for(size_t y(0); y < mWeight.at(x).size(); ++y)
            mWeight.at(x).at(y) += mDWeight.at(x).at(y);
    }
    for(size_t x = 0; x < mBias.size(); ++x) {
        for(size_t y = 0; y < mBias.at(x).size(); ++y)
            mBias.at(x).at(y) += mDBias.at(x).at(y);
    }
}
