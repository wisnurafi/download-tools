#include "Installer.h"
#include "Console.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shellapi.h>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

namespace {

    std::wstring utf8ToWide(const std::string& s) {
        if (s.empty()) return std::wstring();
        int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), NULL, 0);
        std::wstring w(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), &w[0], size);
        return w;
    }

    bool hasExtension(const std::string& path, const std::string& ext) {
        if (path.size() < ext.size()) return false;
        return _stricmp(path.substr(path.size() - ext.size()).c_str(), ext.c_str()) == 0;
    }

    void drawBar(int progress, long elapsed) {
        int pos = progress / 2;
        Console::gotoXY(0, 0);
        std::cout << "\033[34mInstalling... [";
        for (int i = 0; i < 50; ++i) std::cout << (i <= pos ? '#' : ' ');
        std::cout << "] " << progress << "% Time: " << elapsed << "s   \033[0m" << std::flush;
    }
}

namespace Installer {

    int installSilently(const std::string& filePath) {
        std::wstring widePath = utf8ToWide(filePath);

        std::wstring params;
        if (hasExtension(filePath, ".exe")) {
            params = L"/SILENT /VERYSILENT /s /quiet /norestart";
        }
        else if (hasExtension(filePath, ".msi")) {
            params = L"/quiet /qn /norestart";
        }

        SHELLEXECUTEINFOW info = { 0 };
        info.cbSize = sizeof(info);
        info.fMask = SEE_MASK_NOCLOSEPROCESS;
        info.lpVerb = L"open";
        info.lpFile = widePath.c_str();
        info.lpParameters = params.empty() ? nullptr : params.c_str();
        info.nShow = SW_SHOWNORMAL;

        if (!ShellExecuteExW(&info) || info.hProcess == NULL) {
            std::cout << "Gagal menjalankan installer." << std::endl;
            return -1;
        }

        Console::clear();
        auto startTime = std::chrono::steady_clock::now();
        std::thread bar([&]() {
            while (WaitForSingleObject(info.hProcess, 500) == WAIT_TIMEOUT) {
                long elapsed = static_cast<long>(
                    std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::steady_clock::now() - startTime).count());
                int progress = static_cast<int>((elapsed * 100) / 60);
                if (progress > 100) progress = 100;
                drawBar(progress, elapsed);
            }
            });

        WaitForSingleObject(info.hProcess, INFINITE);
        bar.join();

        DWORD exitCode = 0;
        GetExitCodeProcess(info.hProcess, &exitCode);
        CloseHandle(info.hProcess);

        std::cout << std::endl;
        return static_cast<int>(exitCode);
    }

    void openFile(const std::string& filePath) {
        std::wstring widePath = utf8ToWide(filePath);
        ShellExecuteW(NULL, L"open", widePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}
