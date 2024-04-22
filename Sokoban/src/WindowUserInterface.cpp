#ifdef _WIN32
#include "WindowUserInterface.h"
#include "ImageShower.h"
#include <bits/stdc++.h>
using namespace std;

WindowUserInterface::WindowUserInterface(HINSTANCE hInstance, int nCmdShow): window(hInstance, nCmdShow, "SOKOBAN") {
    
}


void WindowUserInterface::start() const {
    //imageShower.insertImage(Image::getImage("man"), {70, 70});
    Point p{70, 70};
    MainMessageHandler::registerMessageCB(WM_KEYDOWN, [&](WPARAM wParam, LPARAM lParam){
        WORD vkCode = LOWORD(wParam);
        switch(vkCode) {
            case 0x57/*W*/:
                p.y -= 5;
                cout << "get W\n";
                break;
            case 0x41/*A*/:
                p.x -= 5;
                cout << "get A\n";
                break;
            case 0x53/*S*/:
                p.y += 5;
                cout << "get S\n";
                break;
            case 0x44/*D*/:
                p.x += 5;
                cout << "get D\n";
                break;
        }
        imageShower.clear();
        imageShower.insertImage(Image::getImage("man"), p);
        imageShower.refreshInstant(window.hwnd);
    });
    imageShower.insertImage(Image::getImage("man"), p);
    window.startMessageLoop();
}

void WindowUserInterface::showStart() {

}





#endif