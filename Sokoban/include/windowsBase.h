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
#include "Image.h"
#include "util.h"

class ButtonLike;
class Image;

class MainProgram {
    inline static bool isRunning = true;
public:
    inline static HINSTANCE hInstance{};
    inline static int nCmdShow{};
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void startMessageLoop();
    static void stopMessageLoop();
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
    friend class MainProgram;
public:
    /*create*/
    static Window* create(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
    static void createMain(std::string name);
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
    class MouseProcesser {
    private:
        class EventHandler {
            std::vector<std::pair<Area, std::function<void()>>> cBs;
        public:
            void insertEvent(Area, std::function<void()> cb);
            void removeEvent(const Area &area);
            void changeEvent(const Area &area, std::function<void()> callBack);
            void process(int x, int y);
            const std::function<bool(Area&, int, int)> trigger;
            EventHandler(std::function<bool(Area&, int, int)> trigger = std::bind(&Area::isIn, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)): trigger(trigger) {}
            friend class MosueProcesser;
        };
    public:
        EventHandler click, moveIn, moveOut{std::bind(&Area::isOut, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)};
        bool process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    }mouseProcesser;
    class ImageShower {
    public:
        HWND &hWnd;
        explicit ImageShower(HWND &hWnd): hWnd(hWnd) {};
        ImageShower& operator=(const ImageShower&) = delete;
        std::map<std::string, std::pair<Image*, Point>> images;
        void show(HDC hdc);
        void clear();
        int insertImage(Image* image, const Point &p);
        /*TEMP*/ int insertImage(std::string name, Image *image, const Point &p);
        int removeImage(std::string name);
        void refreshArea(const Area &area);
        void refreshInstant();
    };
    ImageShower imageShower{hWnd};
    void insertButtonLike(const ButtonLike &button, Point);
};

inline Window *mainWindow = nullptr;


#endif /*AWG*/
