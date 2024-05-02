#pragma once
#include "UserInterface.hpp"

class ConsoleUserInterface: public UserInterface<ConsoleUserInterface> {
public:
    void init();
    void start();
    void showStart();

    class GameObj_base {
    public:
        char exterior;
    };
};