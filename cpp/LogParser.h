#ifndef LOGPARSER_H // standard C++ guard to prevent including file twice
#define LOGPARSER_H

#include <string>
#include <string_view>


struct LogStats {
    long infoCount = 0;
    long warnCount = 0;
    long errorCount = 0;
};

class LogParser {
public:

    LogStats parseContent(const std::string& content);

    LogStats parseContentParallel(const std::string& content);
    LogStats parseContentOpenMP(const std::string& content);
};

#endif