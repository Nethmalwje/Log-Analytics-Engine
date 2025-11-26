package com.example.log_analyzer.service;

import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

@Service
public class FileService {
//    private final String BASE_DIR = System.getProperty("java.io.tmpdir")+"/logsforanalyzer/";
private final String BASE_DIR = "SET Directory";

    public String saveFiles(MultipartFile file) throws IOException {

        Path directory = Paths.get(BASE_DIR);
        if(!Files.exists((directory))){
            Files.createDirectories(directory);
        }

        Path filePath = Paths.get(BASE_DIR, file.getOriginalFilename());

        Files.copy(
                file.getInputStream(),
                filePath,
                StandardCopyOption.REPLACE_EXISTING
        );

        return filePath.toString();
    }
}
