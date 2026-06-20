#pragma once

#include "Catalog.h"
#include <vector>

namespace Menu {
    void showMainMenu();
    void showCategory(const Catalog::Category& cat);
    void showDeveloperInfo();
    void showDownloadInfo(const std::string& savedPath, bool success);
    void showFileManager(const std::vector<Catalog::Category>& cats);
    void showHistory();
}
