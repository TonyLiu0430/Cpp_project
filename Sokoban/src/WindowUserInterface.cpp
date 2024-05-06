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
    mainWindow->keyboardProcesser.insertEvent(VK_SPACE, []()
                                              { cout << "press space\n"; });
    MainProgram::startMessageLoop();
}

void WindowUserInterface::stopMessageLoop() {
    MainProgram::stopMessageLoop();
}

void WindowUserInterface::showBoard(const std::vector<std::vector<GameObj>> &board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            Image *image = nullptr;
            if(board[i][j].isWall()) {
                image = imageManager.getImage("wall");
            } 
            else if(board[i][j].isRoad()) {
                image = imageManager.getImage("road");
            }
            else if(board[i][j].isBox()) {
                if(board[i][j].isCheckPoint()) {
                    image = imageManager.getImage("boxOnCheckPoint");
                }
                else {
                    image = imageManager.getImage("box");
                }
            }
            else if(board[i][j].isCheckPoint()) {
                image = imageManager.getImage("checkPoint");
            }
            
            else if(board[i][j].isPlayer()) {
                image = imageManager.getImage("player");
            }
            if(image == nullptr) {
                throw Exception("Invalid GameObj Data");
            }
            mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, {j * 30, i * 30});
            mainWindow->imageShower.refreshArea({j * 30, i * 30, 30, 30});
        }
    }
}





#endif