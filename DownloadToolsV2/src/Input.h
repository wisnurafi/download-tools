#pragma once

#include <string>

namespace Input {
    // Read an integer from stdin. Re-prompts on non-numeric input.
    // Returns a value; never leaves cin in a failed state.
    int readInt(const std::string& prompt = "");

    // Read a full line (after any prior >> extraction).
    std::string readLine(const std::string& prompt = "");

    // Block until user presses Enter.
    void waitEnter(const std::string& message = "Tekan Enter untuk melanjutkan...");
}
