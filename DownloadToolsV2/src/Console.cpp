#include "Console.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>

namespace Console {

    void init() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }

    void clear() {
        // ANSI clear + home cursor. Avoids spawning cmd.exe via system("cls").
        std::fputs("\033[2J\033[H", stdout);
        std::fflush(stdout);
    }

    void gotoXY(int x, int y) {
        COORD pos;
        pos.X = static_cast<SHORT>(x);
        pos.Y = static_cast<SHORT>(y);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    void showCursor(bool visible) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        if (GetConsoleCursorInfo(hOut, &info)) {
            info.bVisible = visible ? TRUE : FALSE;
            SetConsoleCursorInfo(hOut, &info);
        }
    }

    void setTitle(const char* title) {
        SetConsoleTitleA(title);
    }
}
