#ifdef AWG
#include "windowsBase.h"
#include <bits/stdc++.h>
#include <mutex>
#include "util.h"
#include "Image.h"


#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
using std::cout;

using namespace std;


void MainProgram::startMessageLoop(){
    // Run the message loop.
    /*  消息迴圈  */
    isRunning = true;
    //cout << "start message loop\n";
    MSG msg = {};
    BOOL bRet = 0;
    while (isRunning && (bRet = GetMessage(&msg, NULL, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if(bRet == 0) {
        exit(0);
    }
    /*  消息迴圈  */
}

void MainProgram::stopMessageLoop() {
    isRunning = false;
}

/*訊息處裡函式*/
LRESULT MainProgram::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(mainWindow == nullptr) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    else {
        try {
            return Window::getWindow(hwnd)->process(uMsg, wParam, lParam);
        } catch(Window::FindWindowException &e) {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        throw UnexpectedException("window create error");
    }
    hWndObjs[hWnd] = this;
}

Window* Window::getWindow(HWND hWnd) {
    if(hWndObjs.find(hWnd) == hWndObjs.end()) {
        //throw UnexpectedException("cannot find windows instance");
        cerr << "cannot find windows instance: unexcepted ERROR\n";
        throw FindWindowException();
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

void Window::createMain(std::string name) {
    WNDCLASS windowData = {};
    windowData.lpfnWndProc   = MainProgram::WindowProc;
    windowData.hInstance     = MainProgram::hInstance;
    windowData.lpszClassName = name.c_str();
    windowData.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&windowData);

    // Create the window.
    SetProcessDPIAware();
    mainWindow = create(
        WS_EX_COMPOSITED,                              // Optional window styles.
        name.c_str(),             // Window class
        name.c_str(),             // Window text 左上角文字
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        0, 0, CW_USEDEFAULT, 0,

        NULL,       // Parent window    
        NULL,       // Menu
        MainProgram::hInstance,  // Instance handle
        NULL        // Additional application data
        );

    ShowWindow(mainWindow->getHWnd(), SW_SHOWMAXIMIZED);
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
    //cout << "process " << uMsg << " " << wParam << " " << lParam << endl;

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
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1)); /*填底色*/
            /**********PRINT****************/
            imageShower.show(hdc);
            textShower.show(hdc);
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


void Window::MouseProcesser::EventHandler::insertEvent(Area area, std::function<void()> cb) {
    cBs.push_back({area, cb});
}


void Window::MouseProcesser::EventHandler::removeEvent(const Area &area) {
    if(cBs.size() == 0) {
        return;
    }
    std::lock_guard lock(cBs_Mutex);
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
    if(cBs.size() == 0) {
        return;
    }
    std::unique_lock lock(cBs_Mutex);
    vector<function<void()>> funcs;
    for(auto &[area, cb] : cBs) {
        if(trigger(area, x, y)) {
            funcs.push_back(cb);
        }
    }
    lock.unlock();
    for(auto &cb: funcs) {
        cb();
    }
}

bool Window::MouseProcesser::process(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
    return 0;
}

void Window::KeyboardProcesser::insertEvent(WPARAM vk_code, std::function<void()> callBack) {
    keyCBs[vk_code] = callBack;
}

void Window::KeyboardProcesser::removeEvent(WPARAM vk_code) {
    keyCBs.erase(vk_code);
}

void Window::KeyboardProcesser::clear() {
    keyCBs.clear();
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

void Window::ImageShower::show(HDC hdc) {
    for(auto &[name, imgA]: images) {
        auto &[image, point] = imgA;
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP bg = image->getHBitmap(); 
        SelectObject(mdc,bg);
        BitBlt(hdc, point.x, point.y, image->length, image->width, mdc, 0, 0, SRCAND);
        DeleteDC(mdc);
    }
    for(auto &[name, imgA]: images) {
        auto &[image, point] = imgA;
        image->resetInstances();
    }
}

void Window::ImageShower::clear() {
    images.clear();
}

int Window::ImageShower::insertImage(Image* image, const Point &p) {
    images[image->name] = {image, p};
    return 1;
}

int Window::ImageShower::insertImage(string name, Image* image, const Point &p) {
    images[name] = {image, p};
    return 1;
}

int Window::ImageShower::removeImage(std::string name) {
    if(images.find(name) != images.end()) {
        images.erase(name);
        return 1;
    }
    return 0;
}

void Window::ImageShower::refreshInstant() {
    RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

void Window::ImageShower::refreshArea(const Area &area) {
    RECT rect = {(LONG)area.x, (LONG)area.y, (LONG)area.x + area.length, (LONG)area.y + area.width};
    //cout << "refresh " << " hWnd " << hWnd << " " << area.x << " " << area.y << " " << area.length << " " << area.width << endl;
    InvalidateRect(hWnd, &rect, true);
    UpdateWindow(hWnd);
}

void Window::TextShower::show(HDC hdc) {
    for(auto &[text, area]: texts) {
        RECT rect = {(LONG)area.x, (LONG)area.y, (LONG)area.x + area.length, (LONG)area.y + area.width};
        DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void Window::TextShower::clear() {
    texts.clear();
}

int Window::TextShower::insertText(const std::string &text, const Area &area) {
    texts[text] = area;
    return 1;
}

int Window::TextShower::removeText(std::string text) {
    if(texts.find(text) != texts.end()) {
        texts.erase(text);
        return 1;
    }
    return 0;
}

void Window::TextShower::refreshArea(const Area &area) {
    RECT rect = {(LONG)area.x, (LONG)area.y, (LONG)area.x + area.length, (LONG)area.y + area.width};
    InvalidateRect(hWnd, &rect, true);
    UpdateWindow(hWnd);
}

#endif