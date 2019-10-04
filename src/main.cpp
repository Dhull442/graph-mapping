#include <iostream>
#include "fileReader.h"
#include "processGraphs.h"
using namespace std;

int main(int argc, char **argv) {
  string test = string(argv[argc - 1]);
  fileReader(test);
  writeToFileForMiniSat(test);
  loop(i, 0, correspondanceVector.size()) {
    cout<<i+1<<" ";
    loop(j, 0, correspondanceVector[i].size())
      cout<<correspondanceVector[i][j]<<" ";
    cout<<"\n";  
  }
}