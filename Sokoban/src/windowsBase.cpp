#ifdef AWG
#include "windowsBase.h"
#include <windowsx.h>
#include <bits/stdc++.h>
#include "util.h"
#include "Image.h"
using std::cout;

using namespace std;


void MainProgram::startMessageLoop(){
    // Run the message loop.
    /*  消息迴圈  */
    cout << "start message loop\n";
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0 && isRunning)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    /*  消息迴圈  */
}

void MainProgram::stopMessageLoop() {
    isRunning = false;
}

/*訊息處裡函式*/
LRESULT MainProgram::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //cout << hwnd << " " << uMsg << " " << wParam << " " << lParam << endl;
    if(mainWindow == nullptr) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return Window::getWindow(hwnd)->process(uMsg, wParam, lParam);
    //return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
    //hWndObjs[hWnd] = this;
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
    Window *res = new Window(
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
    hWndObjs[res->hWnd] = res;
    return res;
}

void Window::createMain(std::string name) {
    WNDCLASS windowData = {};
    windowData.lpfnWndProc   = MainProgram::WindowProc;
    windowData.hInstance     = MainProgram::hInstance;
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
        MainProgram::hInstance,  // Instance handle
        NULL        // Additional application data
        );

    cout << "Main window hWnd" << mainWindow->getHWnd() << endl;

    ShowWindow(mainWindow->getHWnd(), MainProgram::nCmdShow);
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
            //cout << "print\n";
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
    mouseProcesser.moveIn.insertEvent(area, [=]() {
        if(imageShower.removeImage(button.name + "_before")) {
            imageShower.insertImage(button.after, {p.x, p.y});
            imageShower.refreshArea(hWnd, area);
        }
    });
    mouseProcesser.moveOut.insertEvent(area, [=]() {
        if(imageShower.removeImage(button.name + "_after")) {
            imageShower.insertImage(button.before, {p.x, p.y});
            imageShower.refreshArea(hWnd, area);
        }
    });
    mouseProcesser.click.insertEvent(area, [=]() {
        if(button.tag == ButtonLike::ActionTag::once) {
            mouseProcesser.moveOut.removeEvent(area);
            mouseProcesser.moveIn.removeEvent(area);
            mouseProcesser.click.removeEvent(area);
        }
        button.action();
        imageShower.refreshArea(hWnd, area);
    });
    imageShower.insertImage(button.before, {p.x, p.y});
    imageShower.refreshArea(hWnd, area);
}

void Window::MouseProcesser::EventHandler::insertEvent(Area area, std::function<void()> cb) {
    cout << "insert Mouse Event\n";
    cBs.push_back({area, cb});
}

void Window::MouseProcesser::EventHandler::removeEvent(const Area &area) {
    for (auto it = cBs.begin(); it != cBs.end(); it++) {
        if (it->first == area) {
            cBs.erase(it);
            return;
        }
    }
}

void Window::MouseProcesser::EventHandler::changeEvent(const Area &area, std::function<void()> callBack) {
    removeEvent(area);
    insertEvent(area, callBack);
}

void Window::MouseProcesser::EventHandler::process(int x, int y) {
    for(auto &[area, cb] : cBs) {
        if(trigger(area, x, y)) {
            cb();
        }
    }
}

bool Window::MouseProcesser::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if( !inSet(uMsg, {WM_MOUSEMOVE, WM_LBUTTONUP}) ) {
        return 0;
    }
    int xPos = GET_X_LPARAM(lParam); 
    int yPos = GET_Y_LPARAM(lParam);
    if(uMsg == WM_MOUSEMOVE) {
        moveIn.process(xPos, yPos);
        moveOut.process(xPos, yPos);
        return 1;
    }
    if(uMsg == WM_LBUTTONUP) {
        click.process(xPos, yPos);
        return 1;
    }
    return 1;
}

void Window::KeyboardProcesser::insertEvent(WPARAM vk_code, std::function<void()> callBack) {
    keyCBs[vk_code] = callBack;
}

void Window::KeyboardProcesser::removeEvent(WPARAM vk_code) {
    keyCBs.erase(vk_code);
}

bool Window::KeyboardProcesser::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(uMsg != WM_KEYDOWN) {
        return 0;
    }
    if(keyCBs.find(wParam) != keyCBs.end()) {
        keyCBs[wParam]();
        return 1;
    }
    return 1;
}
#endif