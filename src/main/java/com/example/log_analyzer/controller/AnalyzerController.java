package com.example.log_analyzer.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api")
public class AnalyzerController {

    @GetMapping("/health")
    public ResponseEntity<String> health(){
        return new ResponseEntity<>("log analyser is running", HttpStatus.OK);
    }
}
