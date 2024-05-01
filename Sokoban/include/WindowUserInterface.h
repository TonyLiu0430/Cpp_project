#pragma once
#ifdef AWG

#include "UserInterface.hpp"
#include "windowsBase.h"


class WindowUserInterface: public UserInterface<WindowUserInterface> {
public:
    WindowUserInterface();
    void init();
    void start();
    void showStart();
};

#endif