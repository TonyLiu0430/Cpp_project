#ifdef AWG
#include "windowsBase.h"
#include <bits/stdc++.h>
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
    cout << "start message loop\n";
    MSG msg = {};
    while (isRunning && GetMessage(&msg, NULL, 0, 0) > 0)
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
    windowData.hCursor       = LoadCursor(NULL, IDC_ARROW);

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

    ShowWindow(mainWindow->getHWnd(), MainProgram::nCmdShow);
    //cerr << "CREATE MAIN\n";
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
            imageShower.refreshArea(area);
        }
    });
    mouseProcesser.moveOut.insertEvent(area, [=]() {
        if(imageShower.removeImage(button.name + "_after")) {
            imageShower.insertImage(button.before, {p.x, p.y});
            imageShower.refreshArea(area);
        }
    });
    mouseProcesser.click.insertEvent(area, [=]() {
        if(button.tag == ButtonLike::ActionTag::once) {
            mouseProcesser.moveOut.removeEvent(area);
            mouseProcesser.moveIn.removeEvent(area);
            mouseProcesser.click.removeEvent(area);
            if(imageShower.removeImage(button.name + "_before") | imageShower.removeImage(button.name + "_after")) {
                imageShower.refreshArea(area);
            }
        }
        button.action();
        //imageShower.refreshArea(hWnd, area);
    });
    imageShower.insertImage(button.before, {p.x, p.y});
    imageShower.refreshArea(area);
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
    /*TODO改成用數字定位*/
    images[image->name] = {image, p};
    return 1;
}

int Window::ImageShower::insertImage(string name, Image* image, const Point &p) {
    /*TODO改成用數字定位*/
    images[name] = {image, p};
    return 1;
}

int Window::ImageShower::removeImage(std::string name) {
    if(images.find(name) != images.end()) {
        cout << "remove " << name << endl;
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
#endif