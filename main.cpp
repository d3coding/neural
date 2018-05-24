#include "headers/matrix.hpp"
#include "headers/tools.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    Tools *tools = new Tools(Tools::useLogicGate(0, 1, 1, 0));
    tools->learn(1000000);
    tools->printResult();
    delete tools;
    Tools::final();
    return 0;
}
