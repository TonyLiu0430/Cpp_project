#include "ConsoleUserInterface.h"

#include <bits/stdc++.h>
#ifdef AWG
#include <conio.h>
#endif
using namespace std;

static char getChar() {
    #ifdef _WIN32
    return _getch();
    #else
    return getchar();
    #endif
}

void ConsoleUserInterface::init() {
    /**/
}

void ConsoleUserInterface::start() {
    cout << "Console UserInterface start" << endl;
    _getch();
}

void ConsoleUserInterface::showStart() {
    cout << "歡迎進入倉庫番遊戲" << endl;
    _getch();
    /*TODO*/
}

void ConsoleUserInterface::startMessageLoop() {
    
}