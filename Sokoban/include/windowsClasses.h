#ifdef AWG
#pragma once
#include "windowsBase.h"

class WindowItem {
    inline static std::map<int, WindowItem*> items{};
protected:
    const HWND hWnd;
    WindowItem();
    ~WindowItem();
};

class Button: public WindowItem {
};

#endif