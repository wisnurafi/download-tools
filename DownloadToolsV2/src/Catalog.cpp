#include "Catalog.h"

#include "vendor/nlohmann/json.hpp"

#include <fstream>

using json = nlohmann::json;

namespace Catalog {

    std::vector<Category> defaults() {
        return {
            { "Browser", {
                { 1, "Google Chrome",   "https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7BB1F81197-15E2-9FAC-4C7F-A86D6E2974D1%7D%26lang%3Den%26browser%3D3%26usagestats%3D0%26appname%3DGoogle%2520Chrome%26needsadmin%3Dprefers%26ap%3Dx86-stable-statsdef_1%26installdataindex%3Dempty/chrome/install/ChromeStandaloneSetup64.exe", "chrome_installer.exe", true, "" },
                { 2, "Microsoft Edge",  "https://c2rsetup.officeapps.live.com/c2r/downloadEdge.aspx?ProductreleaseID=Edge&platform=Default&version=EdgeStable&source=EdgeWebview&Channel=Stable&language=en", "edge_installer.exe", true, "" },
                { 3, "Opera GX",        "https://net.geo.opera.com/opera_gx/stable/windows", "opera_gx_installer.exe", true, "" },
            }},
            { "Sosial Media", {
                { 4, "Discord",   "https://discord.com/api/downloads/distributions/app/installers/latest?channel=stable&platform=win&arch=x64", "discord_installer.exe", true, "" },
                { 5, "Instagram", "", "", false, "Instagram tidak punya installer desktop resmi (PWA / Microsoft Store)." },
                { 6, "WhatsApp",  "https://web.whatsapp.com/desktop/windows/release/x64/WhatsAppSetup.exe", "whatsapp_installer.exe", true, "" },
                { 7, "Telegram",  "https://telegram.org/dl/desktop/win64", "telegram_installer.exe", true, "" },
            }},
            { "Coding Tools", {
                { 8,  "Visual Studio Code", "https://update.code.visualstudio.com/latest/win32-x64-user/stable", "vscode_installer.exe", true, "" },
                { 9,  "Visual Studio",      "https://aka.ms/vs/17/release/vs_community.exe", "visualstudio_installer.exe", true, "" },
                { 10, "IntelliJ IDEA",      "https://download.jetbrains.com/product?code=IIC&latest&distribution=windows", "intellij_installer.exe", true, "" },
                { 11, "Android Studio",     "https://redirector.gvt1.com/edgedl/android/studio/install/2026.1.1.10/android-studio-quail1-patch2-windows.exe", "androidstudio_installer.exe", true, "" },
            }},
            { "VPN", {
                { 12, "Psiphon",   "https://psiphon.ca/psiphon3.exe", "psiphon_installer.exe", true, "" },
                { 13, "Turbo VPN", "", "", false, "Turbo VPN tidak punya installer desktop resmi." },
                { 14, "Proton VPN","https://protonvpn.com/download/ProtonVPN_win_v3.5.1.exe", "proton_vpn_installer.exe", true, "" },
            }},
            { "Media Player", {
                { 15, "VLC Media Player",      "https://get.videolan.org/vlc/3.0.23/win64/vlc-3.0.23-win64.exe", "vlc_installer.exe", true, "" },
                { 16, "PotPlayer",            "https://t1.daumcdn.net/potplayer/PotPlayer/Version/Latest/PotPlayerSetup.exe", "potplayer_installer.exe", true, "" },
                { 17, "Winamp",               "https://download.nullsoft.com/winamp/client/winamp59_3000_full_en-us.exe", "winamp_installer.exe", true, "" },
                { 18, "Windows Media Player", "", "", false, "Windows Media Player sudah bundled dengan Windows." },
            }},
        };
    }

    std::vector<Category> load(const std::string& jsonPath) {
        std::ifstream file(jsonPath);
        if (!file) return defaults();

        try {
            json root = json::parse(file);
            std::vector<Category> cats;
            for (const auto& jc : root.at("categories")) {
                Category cat;
                cat.name = jc.at("name").get<std::string>();
                for (const auto& js : jc.at("items")) {
                    Software sw;
                    sw.id = js.at("id").get<int>();
                    sw.name = js.at("name").get<std::string>();
                    sw.url = js.value("url", "");
                    sw.filename = js.value("filename", "");
                    sw.available = js.value("available", true);
                    sw.note = js.value("note", "");
                    cat.items.push_back(std::move(sw));
                }
                cats.push_back(std::move(cat));
            }
            if (cats.empty()) return defaults();
            return cats;
        }
        catch (const json::exception&) {
            return defaults();
        }
    }

    const Software* findById(const std::vector<Category>& cats, int id) {
        for (const auto& cat : cats) {
            for (const auto& sw : cat.items) {
                if (sw.id == id) return &sw;
            }
        }
        return nullptr;
    }
}
