#include <iostream>
#include "fileReader.h"
#include "processGraphs.h"
using namespace std;

int main(int argc, char **argv) {
  string test = string(argv[argc - 2]);
  cout<<test;
  int read = stoi(argv[argc - 1]);
  cout<<read;
  if(read == 1) {
    fileReaderGraph(test);
    writeToFileForMiniSat(test);
  } else {
    fileReaderSat(test);
  }
  // loop(i, 0, GemailOutgoing.size()) {
  //   cout<<i+1<<" ";
  //   loop(j, 0, GemailOutgoing[i].size())
  //     cout<<GemailOutgoing[i][j]<<" ";
  //   cout<<"\n";  
  // }
}