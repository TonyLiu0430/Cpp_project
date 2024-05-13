#pragma once
#ifdef AWG

//#include "UserInterface.hpp"
#include "windowsBase.h"
#include <vector>
#include <Game.h>
#include <iostream>
#include <string>
#include "filesystemCompatible.h"


class WindowUserInterface {
    std::vector<std::vector<GameObj>> prevBoard;
public:
    WindowUserInterface();
    void startPlay(Game<WindowUserInterface> *game);
    void showStart();
    int boardChoose(const std::vector<fs::path> &boardList);
    void showBoard(const std::vector<std::vector<GameObj>> &board, bool init = false);
    void end();
    void showWin();
    void showLose();
    void startMessageLoop();
    void stopMessageLoop();
};

#endif