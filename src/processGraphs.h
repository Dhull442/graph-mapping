#ifndef PROCESSGRAPHS_INCLUDE
#define PROCESSGRAPHS_INCLUDE
#include <vector>
#include <string>
#include "fileReader.h"

///  Correspondance vector between graphs
/// ith entry denotes which nodes of the Gphone graph, 
/// the (i+1)th node in Gemail graph can map to 
extern std::vector<std::vector<long> > correspondanceVector;

/// This function calculates the possible correspondance vector according to 
/// in and out degrees
extern void calculateCorrespondance();

/// This function writes to file for minisat
void writeToFileForMiniSat(std::string);

#endif
