#include "windowsBase.h"
#include "WindowUserInterface.h"
#include "ConsoleUserInterface.h"
#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

#ifdef AWG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
#else
int main() {
#endif
    #ifdef AWG
    MainProgram::hInstance = hInstance;
    MainProgram::nCmdShow = nCmdShow;
    #endif
    int mode = 0;
    #ifdef AWG
    cout << "按下 0 以控制台(cmd)介面開啟遊戲" << endl;
    cout << "按下 1 以視窗應用程式介面開啟遊戲" << endl;
    cin >> mode;
    #endif
    if(mode == 0) {
        /*TODO*/
        /*
        Game<ConsoleUserInterface> game;
        game.start();
        */
    }
    #ifdef AWG
    else {
        Game<WindowUserInterface> game;
        game.start();
    }
    #endif
    return 0;
}
