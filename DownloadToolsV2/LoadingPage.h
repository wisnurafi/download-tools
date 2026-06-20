#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>

void gotoXY(int x, int y) {
    COORD d;
    d.X = static_cast<SHORT>(x);
    d.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), d);
}

void F_Loading() {
    std::cout << "\n\n\n\n\n\n";

    std::cout << "\t\t\t       ======================================  \n";
    std::cout << "\t\t\t                    DOWNLOAD TOOLS V2       \n";
    std::cout << "\t\t\t       ====================================== \n";
    std::cout << "\t\t\t                      BY WISNU RAFI             \n";
    std::cout << "\t\t\t       ________________________________________  \n";

    char a = static_cast<char>(219);
    gotoXY(45, 14);

    std::cout << "LOADING... " << std::endl;

    gotoXY(37, 16);
    for (int r = 1; r <= 26; r++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
        std::cout << a << std::flush;
    }
    std::cout << std::endl;
}
