#pragma once
#include "UserInterface.h"
#include <memory>

class Game {
    std::unique_ptr<UserInterface> ui;
public:
    Game(std::unique_ptr<UserInterface> &ui);
    void start();
};