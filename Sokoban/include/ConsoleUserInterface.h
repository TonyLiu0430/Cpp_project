#pragma once
//#include "UserInterface.hpp"
#include <vector>
#include <map>
#include <functional>
#include "filesystemCompatible.h"
#include "Game.h"

class ConsoleUserInterface {
    std::map<char, std::function<void(void)>> charCallback;
    void insertCharCallback(char c, std::function<void(void)> callback);
    bool MsgLoopRunning = false;
public:
    //ConsoleUserInterface();
    void startPlay(Game<ConsoleUserInterface> *game);
    void showStart();
    int boardChoose(const std::vector<fs::path> &boardList);
    void showBoard(const std::vector<std::vector<GameObj>> &board);
    void end(Game<ConsoleUserInterface> *game);
    void showWin();
    void showLose();
    void startMessageLoop();
    void stopMessageLoop();
    void stopProgram() {};
};