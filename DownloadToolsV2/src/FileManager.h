#pragma once

#include <string>
#include <vector>

namespace FileManager {
    // Returns the user's Downloads folder with trailing backslash.
    // Falls back to %USERPROFILE%\Downloads\ if the known-folder lookup fails.
    std::string downloadDirectory();

    // Path to the history log file under %APPDATA%\DownloadToolsV2\.
    std::string historyFilePath();

    bool directoryAvailable(const std::string& directory);
    bool fileExists(const std::string& filepath);

    // Returns a non-colliding path: file.exe -> file_1.exe if taken.
    std::string uniqueFilename(const std::string& filepath);

    bool deleteFile(const std::string& filepath);
    bool renameFile(const std::string& oldPath, const std::string& newPath);

    void logHistory(const std::string& filename);
    std::vector<std::string> readHistory();

    std::vector<std::string> listFiles(const std::string& directory);
}
