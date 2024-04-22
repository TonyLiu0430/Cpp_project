#pragma once
#ifdef _WIN32

#include "UserInterface.h"
#include "windowsClasses.h"




class WindowUserInterface: public UserInterface {
public:
    Window window;
    WindowUserInterface(HINSTANCE hInstance, int nCmdShow);
    virtual void start() const override;
    virtual void showStart() override;
};

#endif