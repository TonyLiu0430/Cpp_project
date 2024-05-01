#pragma once
#ifdef AWG

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
#include "util.h"

class ButtonLike;

class MainMudule {
public:
    static HINSTANCE hInstance;
    static int nCmdShow;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void startMessageLoop();
};

class Window {
protected:
    HWND hWnd;
    inline static std::map<HWND, Window* > hWndObjs{};
    Window(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
    virtual ~Window();
    std::map<UINT, std::function <void(WPARAM, LPARAM)>> messageCBs;
    std::map<UINT, std::function <void()>> messageCBs_noArgs;
    bool handleMessage(UINT msg, WPARAM wParam, LPARAM lParam); /*1 success 0 no MessageCB*/
    LRESULT process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    friend class MainMudule;
    std::map<UINT, std::function <void(WPARAM, LPARAM)>> messageCBs{};
    std::map<UINT, std::function <void()>> messageCBs_noArgs{};
public:
    /*create*/
    static Window* create(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
    static void createMain(string name);
    /*create*/
    static void remove(Window *window);
    static Window* getWindow(HWND hWnd);
    HWND getHWnd();
    void registerMessageCB(UINT msg, std::function <void(WPARAM, LPARAM)> callBack);
    void registerMessageCB(UINT msg, std::function <void()> callBack);
    class KeyboardProcesser {
        std::map<WPARAM, std::function<void()>> keyCBs;
    public:
        void insertEvent(WPARAM vk_code, std::function<void()> callBack);
        void removeEvent(WPARAM vk_code);
        bool process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    }keyboardProcesser;
    /////////////////////////////////////
    class MouseProcesser {
    private:
        class EventHandler {
            std::vector<std::pair<Area, std::function<void()>>> cBs;
        public:
            void insertEvent(Area, std::function<void()> cb);
            void removeEvent(const Area &area);
            void changeEvent(const Area &area, std::function<void()> callBack);
            void process(int x, int y);
            const function<bool(int, int)> judge = bind(&Area::isIn, placeholders::_1, placeholders::_2);
            friend class MosueProcesser;
        };
    public:
        EventHandler moveIn, moveOut, click;
        bool process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    }mouseProcesser;
    void insertButtonLike(const ButtonLike &button, Point);
};

inline Window *mainWindow = nullptr;


#endif /*AWG*/
