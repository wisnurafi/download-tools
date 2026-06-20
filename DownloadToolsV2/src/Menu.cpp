#include "Menu.h"
#include "Console.h"
#include "FileManager.h"
#include "Input.h"

#include <iostream>
#include <iomanip>

namespace {
    const char* PAD = "\t\t\t   ";

    void header(const std::string& title) {
        Console::clear();
        std::cout << PAD << "==================================\n";
        std::cout << PAD << "  " << title << "\n";
        std::cout << PAD << "==================================\n\n";
    }
}

namespace Menu {

    void showMainMenu() {
        header("MENU DOWNLOAD");
        std::cout << PAD << "  1. Browser\n";
        std::cout << PAD << "  2. Sosial Media\n";
        std::cout << PAD << "  3. Coding Tools\n";
        std::cout << PAD << "  4. VPN\n";
        std::cout << PAD << "  5. Media Player\n\n";
        std::cout << PAD << "  ----------------------------------\n";
        std::cout << PAD << "  6. Manage Download\n";
        std::cout << PAD << "  7. History Download\n";
        std::cout << PAD << "  ----------------------------------\n";
        std::cout << PAD << "  0. Exit Program\n";
    }

    void showCategory(const Catalog::Category& cat) {
        header("MENU " + cat.name);
        for (const auto& sw : cat.items) {
            std::cout << PAD << "  " << std::setw(2) << sw.id << ". " << sw.name;
            if (!sw.available) std::cout << "  (tidak tersedia)";
            std::cout << "\n";
        }
    }

    void showDeveloperInfo() {
        std::cout << PAD << "Terimakasih sudah menggunakan tools ini\n";
        std::cout << PAD << "=======================================\n";
        std::cout << PAD << "        DEVELOPER INFORMATION\n";
        std::cout << PAD << "=======================================\n";
        std::cout << PAD << "  Developer : Wisnu Rafi\n";
        std::cout << PAD << "  Github    : github.com/wsnrfidev\n";
        std::cout << PAD << "  Instagram : wisnurafi_\n\n";
    }

    void showDownloadInfo(const std::string& savedPath, bool success) {
        std::cout << PAD << "----------------------------------\n";
        std::cout << PAD << "       DOWNLOAD INFORMATION\n";
        std::cout << PAD << "----------------------------------\n";
        if (success) {
            std::cout << PAD << "Berhasil diunduh ke:\n" << PAD << "  " << savedPath << "\n";
        }
        else {
            std::cout << PAD << "Gagal melakukan unduhan.\n";
        }
    }

    void showFileManager(const std::vector<Catalog::Category>&) {
        header("MANAGEMENT DOWNLOAD");

        std::string dir = FileManager::downloadDirectory();
        if (!FileManager::directoryAvailable(dir)) {
            std::cout << PAD << "Direktori tidak tersedia...\n";
            return;
        }

        std::vector<std::string> files = FileManager::listFiles(dir);
        if (files.empty()) {
            std::cout << PAD << "Folder Downloads kosong.\n";
            return;
        }

        std::cout << PAD << "No.  |  Filename\n";
        std::cout << PAD << "-------------------------------------\n";
        for (size_t i = 0; i < files.size(); ++i) {
            std::cout << PAD << std::setw(3) << (i + 1) << "  |  " << files[i] << "\n";
        }

        int choice = Input::readInt("\n" + std::string(PAD) + "Pilih nomor file (0 = kembali): ");
        if (choice <= 0 || static_cast<size_t>(choice) > files.size()) return;

        std::string filePath = dir + files[static_cast<size_t>(choice) - 1];

        Console::clear();
        std::cout << PAD << "1. Hapus file\n";
        std::cout << PAD << "2. Ganti nama file\n";
        std::cout << PAD << "0. Keluar\n";
        int op = Input::readInt("\n" + std::string(PAD) + "Pilihan: ");

        if (op == 1) {
            std::cout << PAD << (FileManager::deleteFile(filePath) ? "File dihapus.\n" : "Gagal menghapus.\n");
        }
        else if (op == 2) {
            std::string newName = Input::readLine(std::string(PAD) + "Nama baru: ");
            std::string newPath = dir + newName;
            std::cout << PAD << (FileManager::renameFile(filePath, newPath) ? "Nama diganti.\n" : "Gagal rename.\n");
        }
    }

    void showHistory() {
        header("HISTORY DOWNLOAD");
        auto lines = FileManager::readHistory();
        if (lines.empty()) {
            std::cout << PAD << "Belum ada riwayat unduhan.\n";
        }
        else {
            for (const auto& line : lines) std::cout << PAD << line << "\n";
        }
    }
}
