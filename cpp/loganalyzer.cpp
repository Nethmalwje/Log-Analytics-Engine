#include <jni.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono> // for timing
#include <omp.h>  //for  max threads check
#include "com_example_log_analyzer_nativebridge_LogAnalyzerNative.h" // The header you just generated
#include "LogParser.h"

using namespace std;
using namespace std::chrono;


JNIEXPORT jstring JNICALL Java_com_example_log_1analyzer_nativebridge_LogAnalyzerNative_analyzeLog
  (JNIEnv *env, jclass clazz, jstring jFilePath) {

    //  Convert Java String path to C++ const char*
    const char *path = env->GetStringUTFChars(jFilePath, nullptr);
    if (path == nullptr) return nullptr; // OutOfMemory check

    string filePath(path);

    // Release the Java string memory immediately
    env->ReleaseStringUTFChars(jFilePath, path);

    // 2. Perform the High-Performance Analysis
    // std::ifstream file(filePath);
    // std::string result;

    // if (file.is_open()) {
    //     std::string line;
    //     long lineCount = 0;
    //     long errorCount = 0;

    //     // Basic parsing loop (We will upgrade this to SIMD later)
    //     while (std::getline(file, line)) {
    //         lineCount++;
    //         if (line.find("ERROR") != std::string::npos) {
    //             errorCount++;
    //         }
    //     }
    auto ioStart = high_resolution_clock::now();
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        return env->NewStringUTF("Error: Could not open file.");
    }

    string content;
    file.seekg(0, ios::end);
    content.resize(file.tellg());
    file.seekg(0, ios::beg);
    file.read(&content[0], content.size());
    file.close();
    auto ioEnd = high_resolution_clock::now();
    long ioTime = duration_cast<milliseconds>(ioEnd - ioStart).count();
    
    LogParser parser;
    LogStats stats = parser.parseContent(content);
    

    // RUN SERIAL
    auto startSerial = high_resolution_clock::now();
    LogStats statsSerial = parser.parseContent(content);
    auto endSerial = high_resolution_clock::now();
    long timeSerial = duration_cast<milliseconds>(endSerial - startSerial).count();

    //  RUN OPENMP
    auto startParallel = high_resolution_clock::now();
    LogStats statsParallel = parser.parseContentOpenMP(content);
    auto endParallel = high_resolution_clock::now();
    long timeParallel = duration_cast<milliseconds>(endParallel - startParallel).count();

    //REPORT GENERATION
    ostringstream report;
    report << "=== HPC LOG ANALYTICS BENCHMARK ===\n";
    report << "File Size: " << (content.size() / 1024 / 1024) << " MB\n";
    report << "Disk Read Time: " << ioTime << "ms\n\n";

    report << "[1] Serial Processing:\n";
    report << "    Time: " << timeSerial << "ms\n";
    report << "    Found: " << statsSerial.errorCount << " Errors\n\n";

    report << "[2] OpenMP Parallel (" << omp_get_max_threads() << " threads):\n";
    report << "    Time: " << timeParallel << "ms\n";
    report << "    Found: " << statsParallel.errorCount << " Errors\n\n";

    // Avoid division by zero
    double speedup = (timeSerial > 0) ? (double)timeSerial / timeParallel : 0.0;
    report << "SPEEDUP: " << speedup << "x FASTER\n";
    
    return env->NewStringUTF(report.str().c_str());
}