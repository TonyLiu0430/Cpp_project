#ifdef AWG
#include "WindowUserInterface.h"
#include "Image.h"
#include <bits/stdc++.h>
using namespace std;

/*
WindowUserInterface::WindowUserInterface() {
    cout << "Window UserInterface start" << endl;
    Window::createMain("SOKOBAN");
}*/

void WindowUserInterface::init() {
    cout << "Window UserInterface start" << endl;
    Window::createMain("SOKOBAN");
}


void WindowUserInterface::start() {
    ButtonLike start("start", []()
                     { cout << "press button\n";
                       MainProgram::stopMessageLoop(); });
    mainWindow->insertButtonLike(start, {500, 300});
    MainProgram::startMessageLoop();
}

void WindowUserInterface::showStart() {
    
}





#endif