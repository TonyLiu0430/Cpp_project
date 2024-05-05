#pragma once
//#include "UserInterface.hpp"
#include <vector>

class ConsoleUserInterface {
public:
    void init();
    void start();
    void showStart();
    void showBoard(const std::vector<std::vector<char>> &board){/*TODO*/};
    void end() {};
    void startMessageLoop();
    void stopMessageLoop() {};
};