#include "windowsClasses.h"
#include "WindowUserInterface.h"
#include "ConsoleUserInterface.h"
#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
#else
int main() {
#endif
    int mode = 0;
    #ifdef _WIN32
    cout << "按下 0 以控制台(cmd)介面開啟遊戲" << endl;
    cout << "按下 1 以視窗應用程式介面開啟遊戲" << endl;
    cin >> mode;
    #endif
    unique_ptr<UserInterface> ui;
    if(mode == 0) {
        ui = make_unique<ConsoleUserInterface>();
    }
    #ifdef _WIN32
    else {
        ui = make_unique<WindowUserInterface>(hInstance, nCmdShow);
    }
    #endif
    Game game(ui);
    game.start();
    return 0;
}
