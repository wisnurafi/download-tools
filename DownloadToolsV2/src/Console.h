#pragma once

namespace Console {
    // Enable UTF-8 codepage + VT (ANSI escape) processing. Call once at startup.
    void init();

    void clear();
    void gotoXY(int x, int y);
    void showCursor(bool visible);
    void setTitle(const char* title);
}
