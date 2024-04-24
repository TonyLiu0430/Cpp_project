#include "KeyboardGetter.h"
#include <bits/stdc++.h>
#ifdef AWG
#include <conio.h>
#endif
using namespace std;

void KeyboardGetter::insertCharEvent(char ch, string prompt, std::function<void(void)> callbackFunc) {
    assert(called == false);
    charCallback[ch] = callbackFunc;
    charPrompt.push_back(prompt);
}

void KeyboardGetter::insertCharEvent(char ch, vector<string> prompts, std::function<void(void)> callbackFunc) {
    assert(called == false);
    charCallback[ch] = callbackFunc;
    for (const auto &prompt : prompts) {
        charPrompt.push_back(prompt);
    }
}

void KeyboardGetter::get() {
    assert(called == false);
    called = true;
    for (int i = 0; i < charPrompt.size(); i++) {
        printer.insertLine(charPrompt[i]);
    }
    printer.print();
    while(true) {
        char input;
        #ifdef AWG
        input = getch();
        #else
        input = getchar();
        if(input != '\n') {
            getchar(); //must be \n
        }
        #endif
        if(input == '\r') {
            input = '\n';
        }
        if(charCallback.find(input) != charCallback.end()) {
            charCallback[input]();
            return;
        }
        printer.insertLineInstant("無效輸入 請再輸入一次");
    }
}

KeyboardGetter::~KeyboardGetter() {
    assert(called == true);
}