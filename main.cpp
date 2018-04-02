#include "headers/matrix.hpp"
#include "headers/tools.hpp"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc,char* argv[]){
  Tools *tools = new Tools(2,1,2,1);
  tools->getMatrix()->defRate(0.1);
  tools->setLogicGate(0,1,1,0);
  tools->defIterations(4000000);
  tools->learn(1);
  tools->printResult();
  delete tools;
  return 0;
}
