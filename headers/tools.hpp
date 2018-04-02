#ifndef TOOLS_H
#define TOOLS_H

#include "matrix.hpp"
#include <thread>

class Tools{
  public:
    Tools(){}
    Tools(unsigned int a,unsigned int b,unsigned int c,unsigned int d):m(new Matrix(a,b,c,d)){}
    Tools(string presetName);
    ~Tools();
    void defIterations(int x);
    void learn(bool sElapsed = 0);

    void tLearn(bool sElapsed);
    void tJoin();
    void tStop();

    void addInputs(double value);
    void addInputs(double *value[]);
    void setMatrix(Matrix *newMatrix);
    Matrix *getMatrix();
    void makeCalc(bool ask, bool logic = false);
    bool savePreset(string presetName, bool show = 0);
    bool loadPreset(string fileDirectory);
    bool saveFile(string fileName,bool show = 0);
    bool loadFile(string fileName);
    void printResult();
    void setLogicGate(double a,double b,double c,double d);
  private:
    vector<double> input;
    int iterations;
    Matrix *m;
    std::thread *mThread;
};

#endif // TOOLS_H
