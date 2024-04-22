#pragma once
#ifdef _WIN32

#include "UserInterface.h"
#include "windowsBase.h"




class WindowUserInterface: public UserInterface {
public:
    MainWindow window;
    WindowUserInterface(HINSTANCE hInstance, int nCmdShow);
    virtual void start() const override;
    virtual void showStart() override;
};

#endif