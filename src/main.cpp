#include <iostream>
#include "fileReader.h"
#include "processGraphs.h"
using namespace std;

int main(int argc, char **argv) {
  string test = string(argv[argc - 1]);
  fileReader(test);
  writeToFileForMiniSat(test);
}