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
                     { cout << "press button\n"; });
    mainWindow->insertButtonLike(start, {100, 100});
}

void WindowUserInterface::showStart() {

}





#endif