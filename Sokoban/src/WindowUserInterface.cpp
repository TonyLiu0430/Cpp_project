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

void WindowUserInterface::startMessageLoop() {
    MainProgram::startMessageLoop();
}

void WindowUserInterface::stopMessageLoop() {
    MainProgram::stopMessageLoop();
}

void WindowUserInterface::showBoard(const std::vector<std::vector<char>> &board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            Image *image = nullptr;
            if(board[i][j] == '/') {
                image = imageManager.getImage("wall");
            } else if(board[i][j] == '-') {
                image = imageManager.getImage("road");
            } else if(board[i][j] == '0') {
                image = imageManager.getImage("man");
            } else if(board[i][j] == '1') {
                image = imageManager.getImage("box");
            } else if(board[i][j] == '2') {
                image = imageManager.getImage("checkPoint");
            } else if(board[i][j] == '3') {
                image = imageManager.getImage("boxOnCheckPoint");
            }
            mainWindow->imageShower.insertImage(image, {j * 30, i * 30});
            mainWindow->imageShower.refreshArea({j * 30, i * 30, 30, 30});
        }
    }
}





#endif