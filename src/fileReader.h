#ifndef FILEREADER_INCLUDE
#define FILEREADER_INCLUDE
#include <vector>
#define loop(i, start, end) for(long i = start; i < end; i++)

/// Gemail (smaller) graph (contains according to incoming edges)
extern std::vector<std::vector<long> > GemailIncoming;

/// Gphone (larger) graph (contains according to incoming edges)
extern std::vector<std::vector<long> > GphoneIncoming;

/// Gemail (smaller) graph (contains according to outgoing edges)
extern std::vector<std::vector<long> > GemailOutgoing;

/// Gphone (larger) graph (contains according to ougoing edges)
extern std::vector<std::vector<long> > GphoneOutgoing;

/// vector which contains id of isolated nodes of Gphone
extern std::vector<long> GphoneIsolated;

/// vector which contains id of isolated nodes of Gemail
extern std::vector<long> GemailIsolated;

/// This function reads the .graphs file
/// Parameters:
/// - fileName: name of the file to be read
extern void fileReaderGraph(std::string);

/// This function reads the .satoutput file and create mapping file
/// Parameters:
/// - fileName: name of the file to be read
extern void fileReaderSat(std::string);

#endif
