#pragma once
//#include "UserInterface.hpp"
#include <vector>
#include "Game.h"

class ConsoleUserInterface {
public:
    void init();
    void start(Game<ConsoleUserInterface> *game);
    void showStart();
    void showBoard_init(const std::vector<std::vector<GameObj>> &board){};
    void showBoard(const std::vector<std::vector<GameObj>> &board){};
    void showWin(){};
    void showLose(){};
    void end() {};
    void startMessageLoop();
    void stopMessageLoop() {};
};