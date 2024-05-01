#include "ConsoleUserInterface.h"

#include <bits/stdc++.h>
#ifdef AWG
#include <conio.h>
#endif
using namespace std;

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