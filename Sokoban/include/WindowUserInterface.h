#pragma once
#ifdef AWG

//#include "UserInterface.hpp"
#include "windowsBase.h"


class WindowUserInterface {
public:
    void init();
    void start();
    void showStart();
    void showBoard(const std::vector<std::vector<char>> &board);
    void end();
    void startMessageLoop();
    void stopMessageLoop();
};

#endif