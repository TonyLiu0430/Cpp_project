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
    //cerr << "1111";
}


void WindowUserInterface::showStart() {
    ButtonLike start("start", []()
                     { cout << "press button\n";
                       MainProgram::stopMessageLoop(); });
    mainWindow->insertButtonLike(start, {500, 300});
    //cerr << "1111";
    MainProgram::startMessageLoop();
}

void WindowUserInterface::start(Game<WindowUserInterface> *game) {
    const WPARAM VK_W = 0x57;
    const WPARAM VK_A = 0x41;
    const WPARAM VK_S = 0x53;
    const WPARAM VK_D = 0x44;
    mainWindow->keyboardProcesser.insertEvent(VK_W, [=]() {
        try {
            Index to{-1, 0};
            game->move(game->player, to);
            game->player += to;
        } catch (Game<WindowUserInterface>::InvalidMoveException &e) {
            /***/
        }
        showBoard(game->board);
    });
    mainWindow->keyboardProcesser.insertEvent(VK_A, [=]() {
        try {
            Index to{0, -1};
            game->move(game->player, to);
            game->player += to;
        } catch (Game<WindowUserInterface>::InvalidMoveException &e) {
            /***/
        }
        showBoard(game->board);
    });
    mainWindow->keyboardProcesser.insertEvent(VK_S, [=]() {
        try {
            Index to{1, 0};
            game->move(game->player, to);
            game->player += to;
        } catch (Game<WindowUserInterface>::InvalidMoveException &e) {
            /***/
        }
        showBoard(game->board);
    });
    mainWindow->keyboardProcesser.insertEvent(VK_D, [=]() {
        try {
            Index to{0, 1};
            game->move(game->player, to);
            game->player += to;
        } catch (Game<WindowUserInterface>::InvalidMoveException &e) {
            /***/
        }
        showBoard(game->board);
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
            mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, {j * 30, i * 30});
            //mainWindow->imageShower.refreshArea({j * 30, i * 30, 30, 30});
        }
    }
    prevBoard = board;
    mainWindow->imageShower.refreshInstant();
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
                mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, {j * 30, i * 30});
                mainWindow->imageShower.refreshArea({j * 30, i * 30, 30, 30});
            }
        }
    }
    prevBoard = board;
    mainWindow->imageShower.refreshInstant();
}





#endif