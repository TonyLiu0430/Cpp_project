#include "util.h"
#include <bits/stdc++.h>
#include "KeyboardGetter.h"
using namespace std;

int random(int range) {
    return rand() % range;
}

int random(int start, int end) {
    return (rand() % (end - start) + start) + 1;
}

int getSafeNumInput(int start, int end) {
    int res = 0;
    cin >> res;
    while(res < start || res > end || cin.fail()) {
        if(cin.fail()) {
            cin.clear();
            cin.sync();
        }
        printer.insertLineInstant("請輸入範圍內數字" + to_string(start) + " ~ " + to_string(end));
        cin >> res;
    }
    return res;
}

void pressEnterContinue() {
    KeyboardGetter keyboardGetter;
    keyboardGetter.insertCharEvent('\n', (vector<string>){"", "->按下 enter鍵 繼續"}, []()
                                   { return; });
    keyboardGetter.get();
    clearScreen();
    printer.reset();
}

void clearScreen() {
    #if 1
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    #else
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    #endif
}