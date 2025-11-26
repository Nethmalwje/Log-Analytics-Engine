package com.example.log_analyzer.nativebridge;

public class LogAnalyzerNative {

    static {
        //load native library
        System.loadLibrary("loganalyzer");
    }

    // Accepts file path, returns analysis summary as String
    public static native String analyzeLog(String filePath);

    // Temporary mock for testing before implementing C++
    public static String analyzeLogMock(String filePath) {
        return "Mock analysis: file '" + filePath + "' has 42 errors";
    }
}
