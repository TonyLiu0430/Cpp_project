#pragma once
#ifdef AWG

//#include "UserInterface.hpp"
#include "windowsBase.h"
#include <vector>
#include <Game.h>
#include <iostream>


class WindowUserInterface {
    std::vector<std::vector<GameObj>> prevBoard;
public:
    WindowUserInterface(){
        init();
    }
    void init();
    void start(Game<WindowUserInterface> *game);
    void showStart();
    void showBoard_init(const std::vector<std::vector<GameObj>> &board);
    void showBoard(const std::vector<std::vector<GameObj>> &board);
    void end(){/*TODO*/};
    void showWin(){std::cout << "win\n";/*TODO*/};
    void showLose(){/*TODO*/};
    void startMessageLoop();
    void stopMessageLoop();
};

#endif