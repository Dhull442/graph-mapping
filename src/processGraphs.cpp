#include "processGraphs.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

vector<vector<long> > correspondanceVector;
vector<vector<long> > notCorrespondanceVector;

vector<vector<long> > GphoneCorrespondanceVector;

/// This function tells if id i can map to id j
bool canMap(long i, long j) {
  return GphoneIncoming[j - 1].size() >= GemailIncoming[i - 1].size() &&
      GphoneOutgoing[j - 1].size() >= GemailOutgoing[i - 1].size();
}

// TODO: make this function more effecient
void calculateCorrespondance() {
  vector<long> emptyVector;
  GphoneCorrespondanceVector = vector<vector<long>> (GphoneOutgoing.size(), vector<long>());
  loop(i, 0, GemailOutgoing.size()) {
    correspondanceVector.push_back(emptyVector);
    notCorrespondanceVector.push_back(emptyVector);
    loop(j, 0, GphoneOutgoing.size()) {
      bool canMap = GphoneIncoming[j].size() >= GemailIncoming[i].size() &&
      GphoneOutgoing[j].size() >= GemailOutgoing[i].size();
      if(canMap) {
        // correspondanceVector will always be sorted
        correspondanceVector[i].push_back(j + 1);
        GphoneCorrespondanceVector[j].push_back(i + 1);
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

void generateClausesForIsolatedNodes(vector<string> &clauses) {
  string currentClause;
  if(GemailIsolated.size() > GphoneIsolated.size()) {
    currentClause = "0\n";
    clauses.push_back(currentClause);
    return;
  }
  loop(i, 0, GemailIsolated.size()) {
    currentClause = to_string(toVarNumber(GemailIsolated[i], GphoneIsolated[i])) + " 0\n";
    clauses.push_back(currentClause);
  }
}

/// This function adds clauses that removes mapping to same node in Gphone
void generateClausesForGphone(vector<string> &clauses) {
  string currentClause = "";
  loop(i, 0, GphoneOutgoing.size()) {
    long id1,id2;
    loop(j, 0, signed(GphoneCorrespondanceVector[i].size()) - 1) {
      loop(k, j + 1, GphoneCorrespondanceVector[i].size()) {
        // both id1 and id2 cannot map to same i + 1
        id1 = GphoneCorrespondanceVector[i][j];
        id2 = GphoneCorrespondanceVector[i][k];
        currentClause = to_string(-toVarNumber(id1, i + 1)) + " " 
        + to_string(-toVarNumber(id2, i + 1)) + " " + "0\n";
        clauses.push_back(currentClause);
      }
    }
  }
  // for outgoing edges
  // adding clause of neighbors
  // loop(l, 0, GphoneOutgoing.size()) {
  //   loop(i, 0, GphoneOutgoing[l].size()) {
  //     // suppose l + 1 maps to j + 1
  //     loop(j, 0, GemailOutgoing.size()) {
  //       // now i cannot map to a k such that there is no edge
  //       // from j + 1 to k, but there is an edge from l+1 to i
  //       loop(k, 0, GemailOutgoing.size()) {
  //         if(k == j)
  //           continue;
          
  //         // bool edgePresent = binary_search(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1);
  //         bool edgePresent = find(GemailOutgoing[j].begin(), GemailOutgoing[j].end(), k + 1) != GemailOutgoing[j].end();
  //         if(!edgePresent && canMap(j + 1, l + 1) && canMap(k + 1, GphoneOutgoing[l][i])) {
  //           currentClause = to_string(-toVarNumber(j + 1, l + 1)) + " " 
  //           + to_string(-toVarNumber(k + 1, GphoneOutgoing[l][i])) + " " + "0\n";
  //           clauses.push_back(currentClause);
  //         }
  //       }
  //     }
  //   }
  // }
  loop(l, 0, GphoneOutgoing.size()) {
    long startID;
    long endID;
    long temp;
    loop(i, 0, GphoneOutgoing[l].size()) {
      temp = GphoneOutgoing[l][i];
      // suppose l + 1 maps to j + 1
      loop(j, 0, GphoneCorrespondanceVector[l].size()) {
        // now i cannot map to a k such that there is no edge
        // from j + 1 to k, but there is an edge from l+1 to i
        loop(k, 0, GphoneCorrespondanceVector[temp - 1].size()) {
          startID = GphoneCorrespondanceVector[l][j];
          endID = GphoneCorrespondanceVector[temp - 1][k];
          if(startID == endID)
            continue;
          
          // bool edgePresent = binary_search(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1);
          bool edgePresent = find(GemailOutgoing[startID - 1].begin(), GemailOutgoing[startID - 1].end(), endID) != GemailOutgoing[startID - 1].end();
          if(!edgePresent) {
            currentClause = to_string(-toVarNumber(startID, l + 1)) + " " 
            + to_string(-toVarNumber(endID, temp)) + " " + "0\n";
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
  loop(i, 0, signed(currentVarNumbers.size()) - 1) {
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
  bool isClauseEmpty;
  // currentVar maps to i + 1
  loop(i, 0, correspondanceVector[currentVar - 1].size()) {
    long currentID = correspondanceVector[currentVar - 1][i];
    // Now we consider all neighbours of currentVar
    loop(j, 0, GemailOutgoing[currentVar - 1].size()) {
      isClauseEmpty = true;
      currentClause = to_string(-(toVarNumber(currentVar, currentID))) + " ";
      // Now we consider all neighbours of currentID
      loop(k, 0, GphoneOutgoing[currentID - 1].size()) {
        long temp = GemailOutgoing[currentVar - 1][j];
        if(find(correspondanceVector[temp - 1].begin(), correspondanceVector[temp-1].end(), GphoneOutgoing[currentID - 1][k])
        != correspondanceVector[temp-1].end()) {
          currentClause += to_string(toVarNumber(temp, GphoneOutgoing[currentID - 1][k])) + " ";
          isClauseEmpty = false; 
        }      
      }
      currentClause += "0\n";
      clauses.push_back(currentClause);
      if(isClauseEmpty)
        break; 
    }
  }
  // for outgoing edges
  // adding clause of neighbors
  // loop(i, 0, GemailOutgoing[currentVar - 1].size()) {
  //   // suppose currentVar maps to j + 1
  //   loop(j, 0, correspondanceVector[currentVar - 1].size()) {
  //     // now i cannot map to a k such that there is no edge
  //     // from j + 1 to k, but there is an edge from currentVar to i
  //     loop(k, 0, correspondanceVector[GemailOutgoing[currentVar - 1][i] - 1].size()) {
  //       long GphoneStartId = correspondanceVector[currentVar - 1][j];
  //       long GphoneEndId = correspondanceVector[GemailOutgoing[currentVar - 1][i] - 1][k];
  //       if(GphoneStartId == GphoneEndId)
  //         continue;

  //       bool edgePresent = (find(GphoneOutgoing[GphoneStartId - 1].begin(), GphoneOutgoing[GphoneStartId - 1].end(), GphoneEndId) != GphoneOutgoing[GphoneStartId - 1].end());
  //       // bool edgePresent = find(GphoneOutgoing[j].begin(), GphoneOutgoing[j].end(), k + 1) != GphoneOutgoing[j].end();
  //       // && (find(correspondanceVector[currentVar - 1].begin(), correspondanceVector[currentVar - 1].end(), j + 1) != correspondanceVector[currentVar - 1].end())
  //       // && (find(correspondanceVector[GemailOutgoing[currentVar - 1][i] - 1].begin(), correspondanceVector[GemailOutgoing[currentVar - 1][i] - 1].end(), k + 1) != correspondanceVector[GemailOutgoing[currentVar - 1][i] - 1].end());
  //       if(!edgePresent) {
  //         currentClause = to_string(-toVarNumber(currentVar, GphoneStartId)) + " " 
  //         + to_string(-toVarNumber(GemailOutgoing[currentVar - 1][i], GphoneEndId)) + " " + "0\n";
  //         clauses.push_back(currentClause);
  //       }
  //     }
  //   }
  // }
  // for incoming edges
  // adding clause of neighbors
  // loop(i, 0, GemailIncoming[currentVar - 1].size()) {
  //   // suppose currentVar maps to j + 1
  //   loop(j, 0, correspondanceVector[currentVar - 1].size()) {  
  //     // now i cannot map to a k + 1 such that there is no edge
  //     // from k to j + 1, but there is an edge from i to currentVar
  //     loop(k, 0, correspondanceVector[GemailIncoming[currentVar - 1][i] - 1].size()) {
  //       long GphoneStartId = correspondanceVector[currentVar - 1][j];
  //       long GphoneEndId = correspondanceVector[GemailIncoming[currentVar - 1][i] - 1][k];
  //       if(GphoneStartId == GphoneEndId)
  //         continue;
  
  //       bool edgePresent = (find(GphoneIncoming[GphoneStartId - 1].begin(), GphoneIncoming[GphoneStartId - 1].end(), GphoneEndId) != GphoneIncoming[GphoneStartId - 1].end());
  //       // bool edgePresent = find(GphoneIncoming[j].begin(), GphoneIncoming[j].end(), k + 1) != GphoneIncoming[j].end();
  //       // && (find(correspondanceVector[currentVar - 1].begin(), correspondanceVector[currentVar - 1].end(), j + 1) != correspondanceVector[currentVar - 1].end())
  //       // && (find(correspondanceVector[GemailIncoming[currentVar - 1][i] - 1].begin(), correspondanceVector[GemailIncoming[currentVar - 1][i] - 1].end(), k + 1) != correspondanceVector[GemailIncoming[currentVar - 1][i] - 1].end());
  //       if(!edgePresent) {
  //         currentClause = to_string(-toVarNumber(currentVar, GphoneStartId)) + " " 
  //         + to_string(-toVarNumber(GemailIncoming[currentVar - 1][i], GphoneEndId)) + " " + "0\n";
  //         clauses.push_back(currentClause);
  //       }
  //     }
  //   }
  // }
} 

void writeToFileForMiniSat(string fileName) {
  calculateCorrespondance();
  vector<string> clauses;
  generateClausesForIsolatedNodes(clauses);
  // loop(i, 0, correspondanceVector.size()) {
  //   cout<<i+1<<" ";
  //   loop(j, 0, correspondanceVector[i].size())
  //     cout<<correspondanceVector[i][j]<<" ";
  //   cout<<endl;
  // }
  loop(i, 0, GemailOutgoing.size()) {
    if(!binary_search(GemailIsolated.begin(), GemailIsolated.end(), i + 1))
      generateClauses(i + 1, clauses);
  }
  
  generateClausesForGphone(clauses);
  ofstream fileOut(fileName + ".satinput");
  fileOut<<"p cnf "<<(GemailOutgoing.size() * GphoneOutgoing.size())<<" "<<clauses.size()<<"\n";
  loop(i, 0 ,clauses.size()) {
    fileOut<<clauses[i];
  }
  fileOut.close();
}


  

  // loop(i, 0, correspondanceVector[currentVar - 1].size()) {
  //   long currentID = correspondanceVector[currentVar - 1][i];
  //   // Now we consider all neighbours of currentVar
  //   loop(j, 0, GemailIncoming[currentVar - 1].size()) {
  //     currentClause = to_string(-(toVarNumber(currentVar, i + 1))) + " ";
  //     // Now we consider all neighbours of currentID
  //     loop(k, 0, GphoneIncoming[currentID - 1].size()) {
  //       long temp = GemailIncoming[currentVar - 1][j];
  //       if(binary_search(correspondanceVector[temp - 1].begin(), correspondanceVector[temp-1].end(), GphoneIncoming[currentID - 1][k])) {
  //         currentClause += to_string(toVarNumber(temp, GphoneIncoming[currentID - 1][k])) + " "; 
  //       }
  //     }
  //     currentClause += "0\n";
  //     clauses.push_back(currentClause);
  //   }
  // }
