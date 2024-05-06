#pragma once
//#include "UserInterface.hpp"
#include <vector>
#include "Game.h"

class ConsoleUserInterface {
public:
    void init();
    void start();
    void showStart();
    void showBoard(const std::vector<std::vector<GameObj>> &board){/*TODO*/};
    void end() {};
    void startMessageLoop();
    void stopMessageLoop() {};
};