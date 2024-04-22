#ifdef _WIN32
#include "windowsClasses.h"
#include <bits/stdc++.h>
#include "util.h"
#include "ImageShower.h"
using std::cout;

using namespace std;

Window::Window(HINSTANCE hInstance, int nCmdShow, string name): hInstance(hInstance), nCmdShow(nCmdShow), windowName(name) {
    //const char CLASS_NAME[]  = "Sample Window Class";
    windowData.lpfnWndProc   = MainMessageHandler::WindowProc;
    windowData.hInstance     = hInstance;
    windowData.lpszClassName = windowName.c_str();

    RegisterClass(&windowData);

    // Create the window.

    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        windowName.c_str(),             // Window class
        windowName.c_str(),             // Window text 左上角文字
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL) {
        throw Exception("Window Create Error");
    }

    ShowWindow(hwnd, nCmdShow);
}

void Window::startMessageLoop() const {
    // Run the message loop.
    /*  消息迴圈  */
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    /*  消息迴圈  */
}

/*訊息處裡函式*/
LRESULT MainMessageHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(handleMessage(uMsg, wParam, lParam) == 1) {
        return 0;
    }
    switch (uMsg) {
    case WM_DESTROY: /*離開*/
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1)); /*填底色*/
            /**********PRINT****************/
            /**********PRINT****************/
            /**********PRINT****************/
            imageShower.show(hdc);
            /**********PRINT****************/
            /**********PRINT****************/
            /**********PRINT****************/
            EndPaint(hwnd, &ps);
            cout << "print\n";
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); /*windows 默認操作*/
}

void MainMessageHandler::registerMessageCB(UINT msg, function<void(WPARAM, LPARAM)> callBack) {
    if(messageCBs_noArgs.find(msg) != messageCBs_noArgs.end()) {
        messageCBs_noArgs.erase(msg);
    }
    messageCBs[msg] = callBack;
}

void MainMessageHandler::registerMessageCB(UINT msg, function<void()> callBack) {
    if(messageCBs.find(msg) != messageCBs.end()) {
        messageCBs.erase(msg);
    }
    messageCBs_noArgs[msg] = callBack;
}

bool MainMessageHandler::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    if(messageCBs.find(msg) != messageCBs.end()) {
        messageCBs[msg](wParam, lParam);
        return true;
    }
    if(messageCBs_noArgs.find(msg) != messageCBs_noArgs.end()) {
        messageCBs_noArgs[msg]();
        return true;
    }
    return false;
}

#endif