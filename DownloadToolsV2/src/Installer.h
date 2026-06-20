#pragma once

#include <string>

namespace Installer {
    // Run installer silently (auto-detects .exe / .msi silent flags) and
    // show a progress bar until the process exits. Returns process exit code,
    // or -1 if the process could not be started.
    int installSilently(const std::string& filePath);

    // Open a file with its default handler (no shell/cmd involved).
    void openFile(const std::string& filePath);
}
