#include "src/Console.h"
#include "src/Catalog.h"
#include "src/Menu.h"
#include "src/Downloader.h"
#include "src/Installer.h"
#include "src/FileManager.h"
#include "src/Input.h"
#include "LoadingPage.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

    void handleDownload(const Catalog::Software& sw) {
        if (!sw.available) {
            std::cout << "\n   " << sw.note << "\n";
            Input::waitEnter("\n   Tekan Enter untuk kembali...");
            return;
        }

        Downloader::Result res = Downloader::download(sw.url, sw.filename);

        Console::clear();
        if (!res.success) {
            Menu::showDownloadInfo(res.savedPath, false);
            std::cout << "   Error: " << res.error << "\n";
            Input::waitEnter("\n   Tekan Enter untuk kembali...");
            return;
        }

        FileManager::logHistory(sw.filename);

        std::cout << "\t\t\t   Install sekarang? (y = silent install, t = buka manual, lainnya = lewati): ";
        std::string response = Input::readLine();

        if (response == "y") {
            int code = Installer::installSilently(res.savedPath);
            std::cout << (code == 0 ? "\n   Instalasi selesai.\n"
                                    : "\n   Installer keluar dengan kode: " + std::to_string(code) + "\n");
        }
        else if (response == "t") {
            Installer::openFile(res.savedPath);
        }

        Console::clear();
        Menu::showDeveloperInfo();
        Menu::showDownloadInfo(res.savedPath, true);
        Input::waitEnter("\n   Tekan Enter untuk kembali ke menu utama...");
    }
}

int main() {
    Console::init();
    Console::setTitle("Download Tools V2 | WSNRFIDEV");

    if (!Downloader::globalInit()) {
        std::cerr << "Gagal inisialisasi jaringan (libcurl)." << std::endl;
        return 1;
    }

    F_Loading();

    std::string catalogPath = "software_catalog.json";
    std::vector<Catalog::Category> catalog = Catalog::load(catalogPath);

    bool running = true;
    while (running) {
        Menu::showMainMenu();
        int choice = Input::readInt("\n\t\t\t   Masukkan nomor pilihan Anda: ");

        switch (choice) {
        case 1: case 2: case 3: case 4: case 5: {
            size_t idx = static_cast<size_t>(choice) - 1;
            if (idx >= catalog.size()) break;

            const Catalog::Category& cat = catalog[idx];
            Menu::showCategory(cat);
            int sub = Input::readInt("\n\t\t\t   Masukkan nomor pilihan Anda: ");

            const Catalog::Software* sw = Catalog::findById(catalog, sub);
            if (sw) {
                handleDownload(*sw);
            }
            else {
                Input::waitEnter("\n\t\t\t   Pilihan tidak valid. Tekan Enter...");
            }
            break;
        }
        case 6:
            Menu::showFileManager(catalog);
            Input::waitEnter("\n\t\t\t   Tekan Enter untuk kembali...");
            break;
        case 7:
            Menu::showHistory();
            Input::waitEnter("\n\t\t\t   Tekan Enter untuk kembali...");
            break;
        case 0:
            running = false;
            break;
        default:
            Input::waitEnter("\n\t\t\t   Pilihan tidak valid. Tekan Enter...");
            break;
        }
    }

    Console::clear();
    std::cout << "\t\t\t   Menutup program...\n";

    Downloader::globalCleanup();
    return 0;
}
