#ifdef AWG
#include "windowsBase.h"
#include <windowsx.h>
#include <bits/stdc++.h>
#include "util.h"
#include "Image.h"
using std::cout;

using namespace std;

void MainMudule::startMessageLoop(){
    // Run the message loop.
    /*  消息迴圈  */
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    /*  消息迴圈  */
}

/*訊息處裡函式*/
LRESULT MainMudule::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return Window::getWindow(hwnd)->process(uMsg, wParam, lParam);
}

Window::Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam) {
    hWnd = CreateWindowEx(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );
    if(hWnd == NULL) {
        throw Exception("window create error");
    }
    hWndObjs[hWnd] = this;
}

Window* Window::getWindow(HWND hWnd) {
    if(hWndObjs.find(hWnd) == hWndObjs.end()) {
        throw Exception("cannot find windows instance");
    }
    return hWndObjs[hWnd];
}

HWND Window::getHWnd() {
    return hWnd;
}

Window* Window::create(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam) {
    return new Window(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );
}

void Window::createMain(string name) {
    WNDCLASS windowData = {};
    windowData.lpfnWndProc   = MainMudule::WindowProc;
    windowData.hInstance     = MainMudule::hInstance;
    windowData.lpszClassName = name.c_str();

    RegisterClass(&windowData);

    // Create the window.

    mainWindow = create(
        0,                              // Optional window styles.
        name.c_str(),             // Window class
        name.c_str(),             // Window text 左上角文字
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        MainMudule::hInstance,  // Instance handle
        NULL        // Additional application data
        );

    ShowWindow(mainWindow->getHWnd(), MainMudule::nCmdShow);
}

void Window::remove(Window* window) {
    if(window == nullptr) {
        return;
    }
    delete window;
}

Window::~Window() {
    SendMessage(hWnd, WM_CLOSE, 0, 0);
    hWndObjs.erase(hWnd);
}

LRESULT Window::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(mouseProcesser.process(uMsg, wParam, lParam)) {
        return 0;
    }
    if(keyboardProcesser.process(uMsg, wParam, lParam)) {
        return 0;
    }
    if(handleMessage(uMsg, wParam, lParam) == 1) {
        return 0;
    }
    switch (uMsg) {
    case WM_DESTROY: /*離開*/
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1)); /*填底色*/
            /**********PRINT****************/
            /**********PRINT****************/
            /**********PRINT****************/
            imageShower.show(hdc);
            /**********PRINT****************/
            /**********PRINT****************/
            /**********PRINT****************/
            EndPaint(hWnd, &ps);
            cout << "print\n";
        }
        return 0;

    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam); /*windows 默認操作*/
}

bool Window::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
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

void Window::registerMessageCB(UINT msg, function<void(WPARAM, LPARAM)> callBack) {
    if(messageCBs_noArgs.find(msg) != messageCBs_noArgs.end()) {
        messageCBs_noArgs.erase(msg);
    }
    messageCBs[msg] = callBack;
}

void Window::registerMessageCB(UINT msg, function<void()> callBack) {
    if(messageCBs.find(msg) != messageCBs.end()) {
        messageCBs.erase(msg);
    }
    messageCBs_noArgs[msg] = callBack;
}

void Window::insertButtonLike(const ButtonLike &button, Point p) {
    Area area({p.x, p.y}, {button.length, button.width});
    mouseProcesser.insertMoveEvent(area, []()
                               { imageShower.changeImage(button.name, button.after); })
}

void Window::KeyboardProcesser::insertEvent(WPARAM vk_code, std::function<void()> callBack) {
    keyCBs[vk_code] = callBack;
}

void Window::KeyboardProcesser::removeEvent(WPARAM vk_code) {
    keyCBs.erase(vk_code);
}

bool Window::KeyboardProcesser::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if( !inSet(uMsg, {WM_KEYDOWN}) ) {
        return 0;
    }
    if( !inSet(wParam, keyCBs) ) {
        return 0;
    }
    keyCBs[wParam]();
}

void Window::MouseProcesser::insertMoveEvent(const Area &area, std::function<void()> callBack) {
    areaCBs.push_back({area, callBack});
}

void Window::MouseProcesser::removeEvent(const Area &area) {
    for (int i = 0; i < areaCBs.size(); i++ ) {
        if(areaCBs[i].first == area) {
            areaCBs.erase(areaCBs.begin() + i);
            return;
        }
    }
}

bool Window::MouseProcesser::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if( !inSet(uMsg, {WM_MOUSEMOVE}) ) {
        return 0;
    }
    int xPos = GET_X_LPARAM(lParam); 
    int yPos = GET_Y_LPARAM(lParam); 
    for(auto &[area, cb] : areaCBs) {
        if(area.has(xPos, yPos)) {
            cb();
        }
    }
}
#endif