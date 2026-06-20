#include "Input.h"

#include <iostream>
#include <limits>

namespace Input {

    int readInt(const std::string& prompt) {
        int value = 0;
        while (true) {
            if (!prompt.empty()) std::cout << prompt;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            if (std::cin.eof()) return 0; // no more input (piped/closed stdin) -> treat as cancel
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Masukkan angka." << std::endl;
        }
    }

    std::string readLine(const std::string& prompt) {
        if (!prompt.empty()) std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    void waitEnter(const std::string& message) {
        std::cout << message;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
