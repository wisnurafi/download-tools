#pragma once

#include <string>

namespace Downloader {
    // Call once at program start / end (wraps curl_global_init / cleanup).
    bool globalInit();
    void globalCleanup();

    struct Result {
        bool success = false;
        std::string savedPath;   // final unique path the file was written to
        std::string error;       // human-readable error when !success
    };

    // Download url into the Downloads folder as outputFilename.
    // Retries on transient failures with exponential backoff.
    Result download(const std::string& url, const std::string& outputFilename, int maxRetries = 3);
}
