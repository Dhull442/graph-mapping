#include <iostream>
#include "fileReader.h"
#include <vector>
#include <fstream>
using namespace std;

/// Gemail (smaller) graph (contains according to incoming edges)
vector<vector<long> > GemailIncoming;

/// Gphone (larger) graph (contains according to incoming edges)
vector<vector<long> > GphoneIncoming;

/// Gemail (smaller) graph (contains according to outgoing edges)
vector<vector<long> > GemailOutgoing;

/// Gphone (larger) graph (contains according to ougoing edges)
vector<vector<long> > GphoneOutgoing;

/// This function fills a vector with empty vector upto size expectedSize
/// Parameters
/// - isGphone: if the vector to be filled is Gphone
void fillVector(bool isGphone, long expectedSizeOutgoing, long expectedSizeIncoming) {
  vector<long> emptyVector;
  if(isGphone) {
    long size = GphoneOutgoing.size();
    loop(i, size, expectedSizeOutgoing) {
      GphoneOutgoing.push_back(emptyVector);
    }
    size = GphoneIncoming.size();
    loop(i, size, expectedSizeIncoming) {
      GphoneIncoming.push_back(emptyVector);
    }
  } else {
    long size = GemailOutgoing.size();
    loop(i, size, expectedSizeOutgoing) {
      GemailOutgoing.push_back(emptyVector);
    }
    size = GemailIncoming.size();
    loop(i, size, expectedSizeIncoming) {
      GemailIncoming.push_back(emptyVector);
    }
  }
}


void fileReader(string fileName) {
  ifstream inFile(fileName + ".graphs"); 
  long count = 0;
  long start, end;
  bool isGphone = true;
  // Filling the vectors
  while (inFile >> start >> end) {
    if(start == 0 && end == 0) {
      isGphone = false;
      continue;
    }
    fillVector(isGphone, start, end);
    if(isGphone) {
      GphoneOutgoing[start - 1].push_back(end);
      GphoneIncoming[end - 1].push_back(start);
    } else {
      GemailOutgoing[start - 1].push_back(end);
      GemailIncoming[end - 1].push_back(start);
    }
  }
  // making incoming and outgoing of the same size
  long size = max(GphoneOutgoing.size(), GphoneIncoming.size());
  fillVector(true, size, size);
  size = max(GemailOutgoing.size(), GemailIncoming.size());
  fillVector(false, size, size);
}