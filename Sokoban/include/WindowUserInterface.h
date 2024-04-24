#pragma once
#ifdef AWG

#include "UserInterface.h"
#include "windowsBase.h"




class WindowUserInterface: public UserInterface {
public:
    WindowUserInterface();
    virtual void start() const override;
    virtual void showStart() override;
};

#endif