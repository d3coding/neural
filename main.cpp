#include "headers/matrix.hpp"
#include "headers/tools.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    Tools *tools = new Tools(Tools::useLogicGate(0, 1, 1, 0));
    tools->getMatrix()->setRate(0.1);
    tools->learn(100000);
    tools->printResult();
    delete tools;
    Tools::final();
    return 0;
}
