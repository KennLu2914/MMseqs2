#ifndef DBWRITER_H
#define DBWRITER_H

// Written by Martin Steinegger & Maria Hauser mhauser@genzentrum.lmu.de
// 
// Manages ffindex DB write access.
// For parallel write access, one ffindex DB per thread is generated.
// After the parallel calculation is done, all ffindexes are merged into one.
//

#include <string>
#include <vector>

template <typename T> class DBReader;

class DBWriter {
    public:
        static const size_t ASCII_MODE = 0;
        static const size_t BINARY_MODE = 1;

        DBWriter(const char* dataFileName, const char* indexFileName, int threads = 1, size_t mode = ASCII_MODE);

        ~DBWriter();

        void open();

        void close();
    
        char* getDataFileName() { return dataFileName; }
    
        char* getIndexFileName() { return indexFileName; }

        void write(const char *data, size_t dataSize, const char *key, int threadIdx = 0);
    
        void mergeFiles(DBReader<unsigned int>& qdbr, std::vector<std::pair<std::string, std::string> > files);

        void swapResults(std::string inputDb, size_t splitSize);

        void sortDatafileByIdOrder(DBReader<unsigned int>& qdbr);

        static void mergeResults(const char *outFileName, const char *outFileNameIndex, const char **dataFileNames,
                                        const char **indexFileNames, int fileCount);

        void mergeFilePair(const char *inData1, const char *inIndex1, const char *inData2, const char *inIndex2);
private:

    void checkClosed();

    char* dataFileName;
    char* indexFileName;

    FILE** dataFiles;
    FILE** indexFiles;

    char** dataFileNames;
    char** indexFileNames;

    size_t* offsets;

    int threads;

    int closed;

    std::string datafileMode;
};

#endif
