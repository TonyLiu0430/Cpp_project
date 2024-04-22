#pragma once
#include "UserInterface.h"

class ConsoleUserInterface: public UserInterface {
public:
    virtual void start() const override;
    virtual void showStart() override;
};