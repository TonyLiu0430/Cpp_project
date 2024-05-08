#ifdef AWG
#include "WindowUserInterface.h"
#include "Image.h"
#include <bits/stdc++.h>
using namespace std;
using std::cout;


WindowUserInterface::WindowUserInterface() {
    cout << "Window UserInterface start" << endl;
    Window::createMain("SOKOBAN");
}

void WindowUserInterface::showStart() {
    ButtonLike start("start", [](){
        MainProgram::stopMessageLoop();
    });
    ButtonLike::insertToWindow(mainWindow, start, {500, 300});
    MainProgram::startMessageLoop();
}

void WindowUserInterface::startPlay(Game<WindowUserInterface> *game) {
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
    showBoard(game->board, true);
    MainProgram::startMessageLoop();
    /*Loop out*/
    mainWindow->keyboardProcesser.clear();
}

void WindowUserInterface::end() {
    cerr << "end";
    mainWindow->keyboardProcesser.insertEvent(VK_RETURN, []() {
        cerr << "繼續";
        MainProgram::stopMessageLoop(); 
    });
    startMessageLoop();
    /*Loop out*/
    mainWindow->keyboardProcesser.clear();
    mainWindow->imageShower.clear();
    mainWindow->imageShower.refreshInstant();
}

void WindowUserInterface::startMessageLoop() {
    MainProgram::startMessageLoop();
}

void WindowUserInterface::stopMessageLoop() {
    MainProgram::stopMessageLoop();
}

void WindowUserInterface::showBoard(const std::vector<std::vector<GameObj>> &board, bool init) {
    if(init) {
        mainWindow->imageShower.insertImage(imageManager.getImage("playing"), {0, 0});
        ButtonLike surrender("surrender", [=](){
            showLose();
            MainProgram::stopMessageLoop(); 
        });
        ButtonLike::insertToWindow(mainWindow, surrender, {0, Image::statusWidth + 50});
    }

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
            else if(board[i][j].isPlayer()) {
                if(board[i][j].isCheckPoint()) {
                    image = imageManager.getImage("playerOnCheckPoint");
                }
                else {
                    image = imageManager.getImage("player");
                }
            }
            else if(board[i][j].isCheckPoint()) {
                image = imageManager.getImage("checkPoint");
            }
            if(image == nullptr) {
                throw Exception("Invalid GameObj Data");
            }
            Point p = {Image::statusLen + 30 + j * Image::GameObjLen, Image::statusWidth + 30 + i * Image::GameObjLen};
            mainWindow->imageShower.insertImage(to_string(i) + to_string(j), image, p);
        }
    }
    mainWindow->imageShower.refreshInstant();
}

void WindowUserInterface::showWin() {
    ButtonLike::deleteFromWindow(mainWindow, "surrender");
    mainWindow->imageShower.insertImage("win", imageManager.getImage("win"), {Image::statusLen + 300, Image::statusWidth + 300});
    mainWindow->imageShower.insertImage("pressEnter", imageManager.getImage("pressEnter"), {Image::statusLen + 300, Image::statusWidth + 550});
    mainWindow->imageShower.refreshInstant();
}

void WindowUserInterface::showLose() {
    ButtonLike::deleteFromWindow(mainWindow, "surrender");
    mainWindow->imageShower.insertImage("lose", imageManager.getImage("lose"), {Image::statusLen + 300, Image::statusWidth + 300});
    mainWindow->imageShower.insertImage("pressEnter", imageManager.getImage("pressEnter"), {Image::statusLen + 300, Image::statusWidth + 550});
    mainWindow->imageShower.refreshInstant();
}

int WindowUserInterface::boardChoose(const std::vector<std::string> &boardList) {
    int chooseIndex = 0;
    mainWindow->imageShower.insertImage(imageManager.getImage("chooseLevel"), {0, 0});
    mainWindow->imageShower.refreshArea({0, 0, imageManager.getImage("chooseLevel")->length, imageManager.getImage("chooseLevel")->width});
    for(int i = 0; i < boardList.size(); i++) {
        if(imageManager.hasImage("mission" + to_string(i + 1) + "_before") == false){
            cerr << "尚未支援超過"<< i + 1 << "個關卡" << endl;
            cerr << "\t\t" << boardList[i] << "無法顯示\n";
            continue;
        }
        ButtonLike button("mission" + to_string(i + 1), [=, &chooseIndex](){
            cerr << "mission" + to_string(i + 1) << " clicked\n";
            chooseIndex = i;
            MainProgram::stopMessageLoop();
        });
        ButtonLike::insertToWindow(mainWindow, button, {imageManager.getImage("chooseLevel")->length + 50 + i * 90, imageManager.getImage("chooseLevel")->width + 50});
    }
    MainProgram::startMessageLoop();
    /*LOOP OUT*/
    mainWindow->imageShower.clear();
    mainWindow->imageShower.refreshInstant();
    return chooseIndex;
}




#endif