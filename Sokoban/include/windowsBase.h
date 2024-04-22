#pragma once
#ifdef _WIN32

#ifdef UNICODE
#undef UNICODE
#endif
#ifndef ASCII
#define ASCII
#endif

#include <windows.h>
#include <string>
#include <functional>
#include <exception>
#include <map>

//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class MainWindow {
public:
    const HINSTANCE hInstance;
    const int nCmdShow;
    WNDCLASS windowData = {};
    HWND hWnd;
    std::string windowName;

    MainWindow(HINSTANCE hInstance, int nCmdShow, std::string name = "CLASS_NAME");
    void startMessageLoop() const;
    
};

class Window {
    const HWND hWnd;
};

/*All static*/
class MainMessageHandler {
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void registerMessageCB(UINT msg, std::function <void(WPARAM, LPARAM)> callBack);
    static void registerMessageCB(UINT msg, std::function <void()> callBack);
    static bool handleMessage(UINT msg, WPARAM wParam, LPARAM lParam); /*1 success 0 no MessageCB*/
    inline static std::map<UINT, std::function <void(WPARAM, LPARAM)>> messageCBs{};
    inline static std::map<UINT, std::function <void()>> messageCBs_noArgs{};
};



#endif /*_WIN32*/
