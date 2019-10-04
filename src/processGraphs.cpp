#include "processGraphs.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

vector<vector<long> > correspondanceVector;
vector<vector<long> > notCorrespondanceVector;

// TODO: make this function more effecient
void calculateCorrespondance() {
  vector<long> emptyVector;
  loop(i, 0, GemailOutgoing.size()) {
    correspondanceVector.push_back(emptyVector);
    notCorrespondanceVector.push_back(emptyVector);
    loop(j, 0, GphoneOutgoing.size()) {
      bool canMap = GphoneIncoming[j].size() >= GemailIncoming[i].size() &&
      GphoneOutgoing[j].size() >= GemailOutgoing[i].size();
      if(canMap) {
        // correspondanceVector will always be sorted
        correspondanceVector[i].push_back(j + 1);
      } else {
        notCorrespondanceVector[i].push_back(j + 1);
      }
    }
  }
}

/// Converts mapping from id i to id j into variable number
int toVarNumber(long i, long j) {
  return (GphoneIncoming.size() * (i - 1) + j);
}

/// This function adds clauses that removes mapping to same node in Gphone
void generateClausesForGphone(vector<string> &clauses) {
  string currentClause = "";
  loop(i, 0, GphoneOutgoing.size()) {
    loop(j, 0, GemailOutgoing.size() - 1) {
      loop(k, j + 1, GemailOutgoing.size()) {
        // both k + 1 and i + 1 cannot map to same i
        currentClause = to_string(-toVarNumber(j + 1, i + 1)) + " " 
        + to_string(-toVarNumber(k + 1, i + 1)) + " " + "0\n";
        clauses.push_back(currentClause);
      }
    }
  }
  // for outgoing edges
  // adding clause of neighbors
  loop(l, 0, GphoneOutgoing.size()) {
    loop(i, 0, GphoneOutgoing[l].size()) {
      // suppose l + 1 maps to j + 1
      loop(j, 0, GemailOutgoing.size()) {
        // now i cannot map to a k such that there is no edge
        // from j + 1 to k, but there is an edge from l+1 to i
        loop(k, 0, GemailOutgoing.size()) {
          if(k == j)
            continue;
          
          // bool edgePresent = binary_search(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1);
          bool edgePresent = find(GemailOutgoing[j].begin(), GemailOutgoing[j].end(), k + 1) != GemailOutgoing[j].end();
          if(!edgePresent) {
            currentClause = to_string(-toVarNumber(j + 1, l + 1)) + " " 
            + to_string(-toVarNumber(k + 1, GphoneOutgoing[l][i])) + " " + "0\n";
            clauses.push_back(currentClause);
          }
        }
      }
    }
  }

  // for incoming edges
  // adding clause of neighbors
  loop(l, 0, GphoneIncoming.size()) {
    loop(i, 0, GphoneIncoming[l].size()) {
      // suppose l + 1 maps to j + 1
      loop(j, 0, GemailIncoming.size()) {
        // now i cannot map to a k such that there is no edge
        // from k to j + 1, but there is an edge from i to l + 1
        loop(k, 0, GemailIncoming.size()) {
          if(k == j)
            continue;
          
          // bool edgePresent = binary_search(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1);
          bool edgePresent = find(GemailIncoming[j].begin(), GemailIncoming[j].end(), k + 1) != GemailIncoming[j].end();
          if(!edgePresent) {
            currentClause = to_string(-toVarNumber(j + 1, l + 1)) + " " 
            + to_string(-toVarNumber(k + 1, GphoneIncoming[l][i])) + " " + "0\n";
            clauses.push_back(currentClause);
          }
        }
      }
    }
  }
}
// TODO: It's possible that we get repeated clauses
void generateClauses(long currentVar, vector<string> &clauses) {
  vector<long> currentVarNumbers;
  // TODO: can remove if ineffecient
  loop(i, 0, correspondanceVector[currentVar - 1].size()) {
    currentVarNumbers.push_back(toVarNumber(currentVar, correspondanceVector[currentVar - 1][i]));
  }
  string currentClause = "";
  // adding exactly one is true
  loop(i, 0, currentVarNumbers.size()) {
    currentClause += (to_string(currentVarNumbers[i]) + " ");
  }
  currentClause += "0\n";
  clauses.push_back(currentClause);
  loop(i, 0, currentVarNumbers.size() - 1) {
    loop(j, i + 1, currentVarNumbers.size()) {
      currentClause = to_string(-currentVarNumbers[i]) + " " 
      + to_string(-currentVarNumbers[j]) + " " + "0\n";
      clauses.push_back(currentClause);
    }
  }
  // adding not of not correspondance
  loop(i, 0, notCorrespondanceVector[currentVar - 1].size()) {
    currentClause = to_string(-toVarNumber(currentVar, notCorrespondanceVector[currentVar - 1][i])) + " " + "0\n";
    clauses.push_back(currentClause);
  }
  // for outgoing edges
  // adding clause of neighbors
  loop(i, 0, GemailOutgoing[currentVar - 1].size()) {
    // suppose currentVar maps to j + 1
    loop(j, 0, GphoneOutgoing.size()) {
      // now i cannot map to a k such that there is no edge
      // from j + 1 to k, but there is an edge from currentVar to i
      loop(k, 0, GphoneOutgoing.size()) {
        if(k == j)
          continue;
        
        // bool edgePresent = binary_search(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1);
        bool edgePresent = find(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1) != GphoneOutgoing[j].end();
        if(!edgePresent) {
          currentClause = to_string(-toVarNumber(currentVar, j + 1)) + " " 
          + to_string(-toVarNumber(GemailOutgoing[currentVar - 1][i], k + 1)) + " " + "0\n";
          clauses.push_back(currentClause);
        }
      }
    }
  }
  // for incoming edges
  // adding clause of neighbors
  loop(i, 0, GemailIncoming[currentVar - 1].size()) {
    // suppose currentVar maps to j + 1
    loop(j, 0, GphoneIncoming.size()) {
      // now i cannot map to a k such that there is no edge
      // from k to j + 1, but there is an edge from i to currentVar
      loop(k, 0, GphoneIncoming.size()) {
        if(k == j)
          continue;
        // bool edgePresent = binary_search(GphoneIncoming[j].begin(), GphoneIncoming[j].end(), k + 1);
        bool edgePresent = find(GphoneIncoming[j].begin(), GphoneIncoming[j].end(), k + 1) != GphoneIncoming[j].end();
        if(!edgePresent) {
          currentClause = to_string(-toVarNumber(currentVar, j + 1)) + " " 
          + to_string(-toVarNumber(GemailIncoming[currentVar - 1][i], k + 1)) + " " + "0\n";
          clauses.push_back(currentClause);
        }
      }
    }
  }
} 

void writeToFileForMiniSat(string fileName) {
  calculateCorrespondance();
  vector<string> clauses;
  loop(i, 0, GemailOutgoing.size()) {
    generateClauses(i + 1, clauses);
  }
  generateClausesForGphone(clauses);
  ofstream fileOut(fileName + ".satinput");
  fileOut<<"p cnf "<<(GemailOutgoing.size() * GphoneOutgoing.size())<<" "<<clauses.size()<<"\n";
  loop(i, 0 ,clauses.size()) {
    fileOut<<clauses[i];
  }
}

// int main() {
//   fileReader("test2.graphs");
//   calculateCorrespondance();
//   loop(i, 0, GemailOutgoing.size()) {
//     cout<<i+1<<" ";
//     loop(j, 0, GemailOutgoing[i].size())
//       cout<<GemailOutgoing[i][j]<<" ";
//     cout<<"\n";  
//   }

//   loop(i, 0, GphoneOutgoing.size()) {
//     cout<<i+1<<" ";
//     loop(j, 0, GphoneIncoming[i].size())
//       cout<<GphoneIncoming[i][j]<<" ";
//     cout<<"\n";  
//   }
//   vector<string> clauses;
//   generateClauses(3, clauses);
//   loop(i, 0 , clauses.size()) {
//     cout<<clauses[i]<<endl;
//   }
//   writeToFileForMiniSat("test");
//   return 0;
// }