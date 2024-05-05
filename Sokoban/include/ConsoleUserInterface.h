#pragma once
//#include "UserInterface.hpp"

class ConsoleUserInterface {
public:
    void init();
    void start();
    void showStart();
    void showBoard(const std::vector<std::vector<char>> &board);
    void end();
    void startMessageLoop();
    void stopMessageLoop();
};