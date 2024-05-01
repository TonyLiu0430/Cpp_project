#ifdef AWG
#include "WindowUserInterface.h"
#include "Image.h"
#include <bits/stdc++.h>
using namespace std;

WindowUserInterface::WindowUserInterface() {
    Window::createMain("SOKOBAN");
}


void WindowUserInterface::start() const {
    ButtonLike start("start", []()
                     { cout << "press button\n";
                       MainProgram::stopMessageLoop(); });
    mainWindow->insertButtonLike(start, {500, 300});
    MainProgram::startMessageLoop();
}

void WindowUserInterface::showStart() {
    
}





#endif