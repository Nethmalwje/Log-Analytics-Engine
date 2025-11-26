package com.example.log_analyzer.controller;

import com.example.log_analyzer.nativebridge.LogAnalyzerNative;
import com.example.log_analyzer.service.FileService;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;

@RestController
@RequestMapping("/api")
@RequiredArgsConstructor
public class UploadController {
    private final FileService fileSaveService;

    @PostMapping("/upload-log")
    public ResponseEntity<?> uploadLog(@RequestParam("file")MultipartFile file){
        try {
            String savedPath = fileSaveService.saveFiles(file);

            //call C++ native parser
//            String result = LogAnalyzerNative.analyzeLogMock(savedPath);
            String result = LogAnalyzerNative.analyzeLog(savedPath);
            return ResponseEntity.ok("{\"savedAt\":\"" + savedPath + "\", \"analysis\":\"" + result + "\"}");


        } catch (IOException e) {
            return ResponseEntity.badRequest().body("Error: "+e.getMessage());
        }
    }


}
