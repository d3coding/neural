#include "headers/tools.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	Tools::final();
    Tools *tools = new Tools(Tools::useLogicGate(0, 1, 1, 0));
	tools->learn(1000000);
    tools->printResult();
    delete tools;
    Tools::final();
    return 0;
}
