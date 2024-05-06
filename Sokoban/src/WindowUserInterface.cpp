#ifdef AWG
#include "WindowUserInterface.h"
#include "Image.h"
#include <bits/stdc++.h>
using namespace std;
using std::cout;

/*
WindowUserInterface::WindowUserInterface() {
    cout << "Window UserInterface start" << endl;
    Window::createMain("SOKOBAN");
}*/

void WindowUserInterface::init() {
    cout << "Window UserInterface start" << endl;
    Window::createMain("SOKOBAN");
}


void WindowUserInterface::showStart() {
    ButtonLike start("start", [](){
        MainProgram::stopMessageLoop();
    });
    mainWindow->insertButtonLike(start, {500, 300});
    MainProgram::startMessageLoop();
}

void WindowUserInterface::start(Game<WindowUserInterface> *game) {
    const WPARAM VK_W = 0x57;
    const WPARAM VK_A = 0x41;
    const WPARAM VK_S = 0x53;
    const WPARAM VK_D = 0x44;
    mainWindow->keyboardProcesser.insertEvent(VK_W, [=]() {
        game->playMove({-1, 0});
    });
    mainWindow->keyboardProcesser.insertEvent(VK_A, [=]() {
        game->playMove({0, -1});
    });
    mainWindow->keyboardProcesser.insertEvent(VK_S, [=]() {
        game->playMove({1, 0});
    });
    mainWindow->keyboardProcesser.insertEvent(VK_D, [=]() {
        game->playMove({0, 1});
    });
}

void WindowUserInterface::startMessageLoop() {
    MainProgram::startMessageLoop();
}

void WindowUserInterface::stopMessageLoop() {
    MainProgram::stopMessageLoop();
}

void WindowUserInterface::showBoard_init(const std::vector<std::vector<GameObj>> &board) {
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
            mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, {j * Image::GameObjLen, i * Image::GameObjLen});
            mainWindow->imageShower.refreshArea({j * Image::GameObjLen, i * Image::GameObjLen, Image::GameObjLen, Image::GameObjLen});
        }
    }
    prevBoard = board;
    //mainWindow->imageShower.refreshInstant();
}

void WindowUserInterface::showBoard(const std::vector<std::vector<GameObj>> &board) {
    for (int i = 0; i < board.size(); i++ ) {
        for (int j = 0; j < board[i].size(); j++ ) {
            if(board[i][j].data != prevBoard[i][j].data) {
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
                mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, {j * Image::GameObjLen, i * Image::GameObjLen});
                mainWindow->imageShower.refreshArea({j * Image::GameObjLen, i * Image::GameObjLen, Image::GameObjLen, Image::GameObjLen});
            }
        }
    }
    prevBoard = board;
    mainWindow->imageShower.refreshInstant();
}

void WindowUserInterface::showWin() {
    mainWindow->imageShower.insertImage("win", imageManager.getImage("win"), {300, 300});
    mainWindow->imageShower.refreshInstant();
}

int WindowUserInterface::boardChoose(const std::vector<std::string> &boardList) {
    int chooseIndex = 0;
    for(int i = 0; i < boardList.size(); i++) {
        if(i + 1 > 4) {
            cerr << "尚未支援超過4個關卡" << endl;
        }
        ButtonLike button("mission" + to_string(i + 1), [=, &chooseIndex](){
            try{
                chooseIndex = i;
                cerr << "NO ERROR" << endl;
                MainProgram::stopMessageLoop();
            } catch (...) {
                system("pause");
                std::rethrow_exception(std::current_exception());
            }
        });
        mainWindow->insertButtonLike(button, {300 + i * 70, 150});
    }
    MainProgram::startMessageLoop();
    /*LOOP OUT*/
    mainWindow->imageShower.clear();
    mainWindow->imageShower.refreshInstant();
    return chooseIndex;
}




#endif