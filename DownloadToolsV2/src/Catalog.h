#pragma once

#include <string>
#include <vector>

namespace Catalog {

    struct Software {
        int id = 0;
        std::string name;
        std::string url;
        std::string filename;
        bool available = true;   // false => no official desktop installer
        std::string note;        // shown when !available
    };

    struct Category {
        std::string name;
        std::vector<Software> items;
    };

    // Load catalog from jsonPath. If the file is missing or invalid,
    // returns the built-in default catalog.
    std::vector<Category> load(const std::string& jsonPath);

    // Built-in default catalog (compiled in, always works offline).
    std::vector<Category> defaults();

    // Find a software entry by its global id across all categories.
    const Software* findById(const std::vector<Category>& cats, int id);
}
