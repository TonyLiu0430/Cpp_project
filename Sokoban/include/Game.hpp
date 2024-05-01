#pragma once
#include "UserInterface.hpp"
#include <memory>
#include <iostream>

template<class T>
class Game {
    UserInterface<T> ui;
public:
    void start();
};

template<class T>
void Game<T>::start() {
    ui.showStart();
    ui.start();
    std::cout << "end" << std::endl;
}