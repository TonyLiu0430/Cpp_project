#pragma once
#ifdef AWG

//#include "UserInterface.hpp"
#include "windowsBase.h"
#include <vector>
#include <Game.h>
#include <iostream>


class WindowUserInterface {
public:
    WindowUserInterface(){
        init();
    }
    void init();
    void start(Game<WindowUserInterface> *game);
    void showStart();
    int boardChoose(const std::vector<std::string> &boardList);
    void showBoard(const std::vector<std::vector<GameObj>> &board, bool init = false);
    void end();
    void showWin();
    void showLose();
    void startMessageLoop();
    void stopMessageLoop();
};

#endif