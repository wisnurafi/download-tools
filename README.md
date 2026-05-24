# Download Tools V2

A Windows console application written in C++ that helps users quickly download and install popular software (browsers, social media apps, coding tools, VPNs, and media players) directly from a simple text-based menu.

Built on top of [libcurl](https://curl.se/libcurl/) for HTTP downloads and the Win32 Shell API for installer execution.

## Features

- Curated download menu grouped into categories:
  - Browsers (Chrome, Edge, Opera GX)
  - Social Media (Discord, Instagram, WhatsApp, Telegram)
  - Coding Tools (VS Code, Visual Studio, IntelliJ IDEA, Android Studio)
  - VPN (Psiphon, Turbo VPN, Proton VPN)
  - Media Player (VLC, PotPlayer, Winamp, Windows Media Player)
- Real-time download progress bar with speed and estimated time remaining
- Saves downloads to the user's Windows `Downloads` folder automatically
- Auto-renames duplicate files (e.g. `installer.exe` -> `installer_1.exe`)
- Optional silent installation of `.exe` / `.msi` files after download
- Download history log (`download_history.txt`)
- Built-in download manager: rename or delete files in the Downloads folder

## Requirements

- Windows 10 or later
- Visual Studio 2022 (v143 toolset) with the **Desktop development with C++** workload
- C++17
- [vcpkg](https://github.com/microsoft/vcpkg) for dependency management

## Dependencies

| Library | Source |
| ------- | ------ |
| libcurl | vcpkg (`curl`) |
| Win32 / Shell API | Windows SDK (`Shell32.lib`, `Shlwapi.lib`) |

## Setup

1. Clone the repository:

   ```sh
   git clone https://github.com/wsnrfidev/DownloadToolsV2.git
   cd DownloadToolsV2
   ```

2. Install vcpkg (if you don't have it yet) and integrate it with Visual Studio:

   ```sh
   git clone https://github.com/microsoft/vcpkg.git
   .\vcpkg\bootstrap-vcpkg.bat
   .\vcpkg\vcpkg integrate install
   ```

3. Install libcurl:

   ```sh
   .\vcpkg\vcpkg install curl:x64-windows
   ```

4. Open `DownloadToolsV2.sln` in Visual Studio 2022.

5. Select the **x64** platform and either **Debug** or **Release** configuration, then build (`Ctrl+Shift+B`).

6. Run the project (`F5` to debug, `Ctrl+F5` to run without debugging).

## Usage

Once the program starts, you'll see the main menu:

```
==================================
         MENU DOWNLOAD
==================================

  1. Browser
  2. Sosial Media
  3. Coding Tools
  4. VPN
  5. Media Player
  ----------------------------------
  6. Manage Download
  7. History Download
  ----------------------------------
  0. Exit Program
```

1. Pick a category, then choose the software you want to download.
2. Wait for the download to finish; the progress bar will show speed and percentage.
3. When prompted, choose:
   - `y` to install the software automatically (silent install)
   - `t` to open the file manually
4. Use **Manage Download** to rename or delete files inside your Downloads folder.
5. Use **History Download** to view your past downloads.

## Project Structure

```
DownloadToolsV2/
├── DownloadToolsV2.sln              # Visual Studio solution
└── DownloadToolsV2/
    ├── DownloadToolsV2.vcxproj      # Project file
    ├── main.cpp                     # Main program logic
    └── LoadingPage.h                # Splash / loading screen
```

## Notes

- The program is **Windows-only** because it uses Win32 APIs (`SHGetKnownFolderPath`, `ShellExecuteEx`, etc.).
- Some download URLs point to third-party mirrors and may go stale over time. Update the URLs in `main.cpp` if a download fails.
- Silent install support depends on each installer respecting `/SILENT`, `/quiet`, or similar flags. Installers that don't support silent mode will fall back to their normal UI.
- The user-specific `LibraryPath` in the `.vcxproj` (under `Debug|x64`) points to a hardcoded path. You may need to remove it or adjust it for your machine if linking fails.

## Developer

- **Name:** Wisnu Rafi
- **GitHub:** [github.com/wsnrfidev](https://github.com/wsnrfidev)
- **Instagram:** [@wisnurafi_](https://instagram.com/wisnurafi_)

## License

No license has been specified yet. If you plan to make this repository public, consider adding one (for example, [MIT](https://choosealicense.com/licenses/mit/)).
