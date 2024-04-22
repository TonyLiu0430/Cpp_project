#include "ConsoleUserInterface.h"

#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#endif
using namespace std;

void ConsoleUserInterface::start() const {
    cout << "Console UserInterface start" << endl;
    _getch();
}

void ConsoleUserInterface::showStart() {
    cout << "歡迎進入倉庫番遊戲" << endl;
    _getch();
    /*TODO*/
}