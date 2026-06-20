#include "FileManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>

#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdio>

namespace fs = std::filesystem;

namespace {
    std::string wideToUtf8(PCWSTR w) {
        int size = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
        std::string s;
        if (size > 0) {
            s.resize(static_cast<size_t>(size) - 1);
            WideCharToMultiByte(CP_UTF8, 0, w, -1, s.data(), size, NULL, NULL);
        }
        return s;
    }

    std::string knownFolder(REFKNOWNFOLDERID id) {
        PWSTR path = nullptr;
        std::string result;
        if (SUCCEEDED(SHGetKnownFolderPath(id, 0, NULL, &path))) {
            result = wideToUtf8(path);
        }
        if (path) CoTaskMemFree(path);
        return result;
    }

    std::string userProfile() {
        wchar_t buf[MAX_PATH] = { 0 };
        DWORD n = GetEnvironmentVariableW(L"USERPROFILE", buf, MAX_PATH);
        if (n > 0 && n < MAX_PATH) return wideToUtf8(buf);
        return std::string();
    }
}

namespace FileManager {

    std::string downloadDirectory() {
        std::string dir = knownFolder(FOLDERID_Downloads);
        if (!dir.empty()) return dir + "\\";

        std::string profile = userProfile();
        if (!profile.empty()) return profile + "\\Downloads\\";

        return ".\\";
    }

    std::string historyFilePath() {
        std::string appdata = knownFolder(FOLDERID_RoamingAppData);
        if (appdata.empty()) return "download_history.txt";

        std::string dir = appdata + "\\DownloadToolsV2";
        std::error_code ec;
        fs::create_directories(dir, ec);
        return dir + "\\download_history.txt";
    }

    bool directoryAvailable(const std::string& directory) {
        std::error_code ec;
        return fs::is_directory(directory, ec);
    }

    bool fileExists(const std::string& filepath) {
        std::error_code ec;
        return fs::exists(filepath, ec);
    }

    std::string uniqueFilename(const std::string& filepath) {
        std::string unique = filepath;
        int count = 1;
        while (fileExists(unique)) {
            unique = filepath;
            size_t pos = unique.rfind('.');
            std::string suffix = "_" + std::to_string(count++);
            if (pos != std::string::npos) {
                unique.insert(pos, suffix);
            }
            else {
                unique += suffix;
            }
        }
        return unique;
    }

    bool deleteFile(const std::string& filepath) {
        std::error_code ec;
        return fs::remove(filepath, ec) && !ec;
    }

    bool renameFile(const std::string& oldPath, const std::string& newPath) {
        std::error_code ec;
        fs::rename(oldPath, newPath, ec);
        return !ec;
    }

    void logHistory(const std::string& filename) {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm timeinfo;
        localtime_s(&timeinfo, &now);
        char timeBuf[80];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &timeinfo);

        std::ofstream history(historyFilePath(), std::ios::app);
        if (history.is_open()) {
            history << "Downloaded: " << filename << " at " << timeBuf << "\n";
        }
    }

    std::vector<std::string> readHistory() {
        std::vector<std::string> lines;
        std::ifstream history(historyFilePath());
        std::string line;
        while (std::getline(history, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    std::vector<std::string> listFiles(const std::string& directory) {
        std::vector<std::string> files;
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(directory, ec)) {
            if (entry.is_regular_file(ec)) {
                files.push_back(entry.path().filename().string());
            }
        }
        return files;
    }
}
