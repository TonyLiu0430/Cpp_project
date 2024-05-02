#pragma once
#ifdef AWG

#include "UserInterface.hpp"
#include "windowsBase.h"


class WindowUserInterface: public UserInterface<WindowUserInterface> {
public:
    void init();
    void start();
    void showStart();

    class GameObj_base {
    public:
        Image* exterior;
    };
};

#endif