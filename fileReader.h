#ifndef FILEREADER_INCLUDE
#define FILEREADER_INCLUDE
#include "utils.h"
#include <vector>

/// Gemail (smaller) graph (contains according to incoming edges)
extern std::vector<std::vector<long> > GemailIncoming;

/// Gphone (larger) graph (contains according to incoming edges)
extern std::vector<std::vector<long> > GphoneIncoming;

/// Gemail (smaller) graph (contains according to outgoing edges)
extern std::vector<std::vector<long> > GemailOutgoing;

/// Gphone (larger) graph (contains according to ougoing edges)
extern std::vector<std::vector<long> > GphoneOutgoing;

/// This function reads the file 
/// Parameters:
/// - fileName: name of the file to be read
extern void fileReader(std::string);

#endif