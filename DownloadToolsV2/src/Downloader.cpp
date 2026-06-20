#include "Downloader.h"
#include "FileManager.h"
#include "Console.h"

#include <curl/curl.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>

namespace {

    struct ProgressState {
        double lastProgress = 0.0;
        std::chrono::steady_clock::time_point start;
        bool started = false;
    };

    size_t writeData(void* ptr, size_t size, size_t nmemb, void* userdata) {
        std::ofstream* stream = static_cast<std::ofstream*>(userdata);
        size_t bytes = size * nmemb;
        stream->write(static_cast<const char*>(ptr), bytes);
        return bytes;
    }

    int progressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t) {
        ProgressState* st = static_cast<ProgressState*>(clientp);
        if (dltotal <= 0) return 0;

        if (!st->started) {
            st->start = std::chrono::steady_clock::now();
            st->started = true;
        }

        double progress = (dlnow * 100.0) / static_cast<double>(dltotal);
        auto now = std::chrono::steady_clock::now();
        long elapsed = static_cast<long>(
            std::chrono::duration_cast<std::chrono::seconds>(now - st->start).count());

        double speed = 0.0;
        if (elapsed > 0) speed = static_cast<double>(dlnow) / elapsed / 1024.0;

        if (progress - st->lastProgress >= 1.0) {
            long secondsLeft = 0;
            if (progress > 0.0 && elapsed > 0) {
                secondsLeft = static_cast<long>((100.0 - progress) * elapsed / progress);
            }

            int filled = static_cast<int>(progress) / 2;
            Console::gotoXY(0, 0);
            std::cout << "\033[32mDownloading... ["
                << std::string(filled, '#')
                << std::string(50 - filled, ' ') << "] ";
            std::cout << std::fixed << std::setprecision(2) << progress << "% "
                << "Speed: " << std::setw(6) << speed << " KB/s   ";
            std::cout << "Time Left: " << secondsLeft / 60 << "m" << secondsLeft % 60 << "s   \033[0m"
                << std::flush;
            st->lastProgress = progress;
        }
        return 0;
    }

    bool isTransient(CURLcode code) {
        switch (code) {
        case CURLE_COULDNT_CONNECT:
        case CURLE_COULDNT_RESOLVE_HOST:
        case CURLE_OPERATION_TIMEDOUT:
        case CURLE_PARTIAL_FILE:
        case CURLE_RECV_ERROR:
        case CURLE_SEND_ERROR:
        case CURLE_GOT_NOTHING:
            return true;
        default:
            return false;
        }
    }

    CURLcode performOnce(const std::string& url, const std::string& outputPath) {
        std::ofstream out(outputPath, std::ios::binary | std::ios::trunc);
        if (!out) return CURLE_WRITE_ERROR;

        CURL* curl = curl_easy_init();
        if (!curl) return CURLE_FAILED_INIT;

        ProgressState state;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &state);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 60L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "DownloadToolsV2/2.0");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        out.close();
        return res;
    }
}

namespace Downloader {

    bool globalInit() {
        return curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK;
    }

    void globalCleanup() {
        curl_global_cleanup();
    }

    Result download(const std::string& url, const std::string& outputFilename, int maxRetries) {
        Result result;

        std::string dir = FileManager::downloadDirectory();
        if (!FileManager::directoryAvailable(dir)) {
            result.error = "Direktori Downloads tidak tersedia.";
            return result;
        }

        std::string outputPath = FileManager::uniqueFilename(dir + outputFilename);
        result.savedPath = outputPath;

        CURLcode res = CURLE_OK;
        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            Console::clear();
            if (attempt > 1) {
                std::cout << "Percobaan ulang " << attempt << "/" << maxRetries << "...\n";
            }

            res = performOnce(url, outputPath);
            if (res == CURLE_OK) {
                result.success = true;
                std::cout << std::endl;
                return result;
            }

            FileManager::deleteFile(outputPath);

            if (!isTransient(res) || attempt == maxRetries) break;

            int backoff = 1 << (attempt - 1); // 1s, 2s, 4s
            std::this_thread::sleep_for(std::chrono::seconds(backoff));
        }

        result.error = curl_easy_strerror(res);
        return result;
    }
}
