#include "LogParser.h"
#include <iostream>
#include <omp.h>

using namespace std;

LogStats processChunk(string_view chunk) {
    LogStats stats;
    size_t start = 0;
    while (start < chunk.size()) {
        size_t end = chunk.find('\n', start);
        if (end == string_view::npos) {
            end = chunk.size();
        }
        string_view line = chunk.substr(start, end - start);

        if (line.find("ERROR") != string_view::npos) {
            stats.errorCount++;
        } else if (line.find("WARN") != string_view::npos) {
            stats.warnCount++;
        } else if (line.find("INFO") != string_view::npos) {
            stats.infoCount++;
        }
        start = end + 1;
    }
    return stats;
}


LogStats LogParser::parseContent(const string& content){
    // Pass the whole file as one chunk
    return processChunk(string_view(content));
}

LogStats LogParser::parseContentOpenMP(const string& content){
    LogStats totalStats;
    size_t fileSize = content.size();

    #pragma omp parallel
    {
        //  Partitioning (Decomposition based on Thread ID)
        int tid = omp_get_thread_num();       
        int numThreads = omp_get_num_threads(); 

        size_t chunkSize = fileSize / numThreads;
        size_t startPos = tid * chunkSize;
        // Ensure last thread picks up the slack (rounding errors)
        size_t endPos = (tid == numThreads - 1) ? fileSize : startPos + chunkSize;

        //  Boundary Alignment (Critical for text processing)
        if (startPos > 0) {
            size_t nextNL = content.find('\n', startPos);
            if (nextNL != string::npos) startPos = nextNL + 1;
        }
        
        if (endPos < fileSize) {
            size_t nextNL = content.find('\n', endPos);
            if (nextNL != string::npos) endPos = nextNL + 1;
        }

        // Local Processing
        LogStats localStats; 
        
        // Safety check: ensure start is still before end after alignment
        if (startPos < endPos) {
            string_view chunk(content.data() + startPos, endPos - startPos);
            localStats = processChunk(chunk);
        }

        //Aggregation (The "Reduce" Step) via Mutual Exclusion
        // 'critical' ensures only one thread updates totalStats at a time
        #pragma omp critical
        {
            totalStats.infoCount += localStats.infoCount;
            totalStats.warnCount += localStats.warnCount;
            totalStats.errorCount += localStats.errorCount;
        }
    }
    return totalStats;
}