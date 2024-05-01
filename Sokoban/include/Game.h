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

