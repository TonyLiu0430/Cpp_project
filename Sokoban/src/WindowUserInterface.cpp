#ifdef AWG
#include "WindowUserInterface.h"
#include "Image.h"
#include <bits/stdc++.h>
using namespace std;
using std::cout;


WindowUserInterface::WindowUserInterface() {
    Window::createMain("SOKOBAN");
}

void WindowUserInterface::showStart() {
    ButtonLike start("start", [](){
        MainProgram::stopMessageLoop();
    });
    ButtonLike::insertToWindow(mainWindow, start, {800, 410});
    MainProgram::startMessageLoop();
}

void WindowUserInterface::startPlay(Game<WindowUserInterface> *game) {
    const WPARAM VK_W = 0x57;
    const WPARAM VK_A = 0x41;
    const WPARAM VK_S = 0x53;
    const WPARAM VK_D = 0x44;
    const WPARAM VK_R = 0x52;
    const WPARAM VK_Q = 0x51;
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
    mainWindow->keyboardProcesser.insertEvent(VK_R, [=](){
        game->retToPrev(); 
    });
    mainWindow->keyboardProcesser.insertEvent(VK_Q, [=](){
        game->status = GameStatus::lose;
        MainProgram::stopMessageLoop(); 
    });
    ButtonLike ret("return", [=](){
        game->retToPrev(); 
    }, ButtonLike::ActionTag::repeat);
    ButtonLike::insertToWindow(mainWindow, ret, {0, Image::statusWidth + 50});

    mainWindow->imageShower.insertImage(imageManager.getImage("playing"), {0, 0});
    ButtonLike surrender("surrender", [=](){
        game->moveRecord.push_back({});
        game->status = GameStatus::lose;
        MainProgram::stopMessageLoop(); 
    });
    ButtonLike::insertToWindow(mainWindow, surrender, {0, Image::statusWidth + 250});

    showBoard(game->board, true);
    startMessageLoop();
    /*Loop out*/
    mainWindow->keyboardProcesser.clear();
}

void WindowUserInterface::end(Game<WindowUserInterface> *game) {
    ButtonLike::deleteFromWindow(mainWindow, "surrender");
    ButtonLike::deleteFromWindow(mainWindow, "return");
    Image *endStatusImage = nullptr;
    if(game->status == GameStatus::win) {
        endStatusImage = imageManager.getImage("win");
    }
    else if(game->status == GameStatus::lose) {
        endStatusImage = imageManager.getImage("lose");
        const WPARAM VK_R = 0x52;
        function<void(void)> retToPrevFunc = [=]() {
            game->status = GameStatus::playing;
            game->retToPrev();
            MainProgram::stopMessageLoop();
        };
        mainWindow->keyboardProcesser.insertEvent(VK_R, retToPrevFunc);
        ButtonLike ret("return", retToPrevFunc);
        ButtonLike::insertToWindow(mainWindow, ret, {0, Image::statusWidth + 50});
    }
    mainWindow->imageShower.insertImage(endStatusImage, {Image::statusLen + 300, Image::statusWidth + 300});
    mainWindow->imageShower.insertImage("pressEnter", imageManager.getImage("pressEnter"), {Image::statusLen + 300, Image::statusWidth + 550});
    mainWindow->imageShower.refreshInstant();
    mainWindow->keyboardProcesser.insertEvent(VK_RETURN, []() {
        MainProgram::stopMessageLoop(); 
    });
    startMessageLoop();
    /*Loop out*/
    ButtonLike::deleteFromWindow(mainWindow, "return");
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
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            if(init || board[i][j] != prevBoard[i][j]) {
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
                    throw UnexpectedException("Invalid GameObj Data");
                }
                Point p = {Image::statusLen + 30 + j * Image::GameObjLen, Image::statusWidth + 30 + i * Image::GameObjLen};
                mainWindow->imageShower.insertImage(to_string(i) + "-" + to_string(j), image, p);
                if(!init) {
                    mainWindow->imageShower.refreshArea({p, {Image::GameObjLen, Image::GameObjLen}});
                }
            }
        }
    }
    if(init) {
        mainWindow->imageShower.refreshInstant();
    }
    this->prevBoard = board;
}

int WindowUserInterface::boardChoose(const vector<fs::path> &boardList) {
    int chooseIndex = 0;
    int hasImageCnt = 0;
    int defaultCnt = 0;
    mainWindow->imageShower.insertImage(imageManager.getImage("chooseLevel"), {0, 0});
    mainWindow->imageShower.refreshArea({0, 0, imageManager.getImage("chooseLevel")->length, imageManager.getImage("chooseLevel")->width});
    for(int i = 0; i < boardList.size(); i++) {
        string name = boardList[i].filename().string();
        name = name.substr(0, name.find_last_of('.'));
        if(imageManager.hasImage(name + "_before")) {
            Point coord = {imageManager.getImage("chooseLevel")->length + 50 + hasImageCnt * 100, imageManager.getImage("chooseLevel")->width + 50};
            hasImageCnt++;
            ButtonLike button(name, [=, &chooseIndex](){
                chooseIndex = i;
                MainProgram::stopMessageLoop();
            });
            ButtonLike::insertToWindow(mainWindow, button, coord);
        }
    }
    for(int i = 0; i < boardList.size(); i++) {
        string name = boardList[i].filename().string();
        name = name.substr(0, name.find_last_of('.'));
        if(!imageManager.hasImage(name + "_before")) {
            Point coord = {imageManager.getImage("chooseLevel")->length + 50 + defaultCnt * 100, imageManager.getImage("chooseLevel")->width + 150};
            defaultCnt++;
            ButtonLike button(name, "default", [=, &chooseIndex](){
                    chooseIndex = i;
                    MainProgram::stopMessageLoop();
                }, 
                ButtonLike::ActionTag::once, 
                ButtonLike::ButtonStyle::withText
            );
            ButtonLike::insertToWindow(mainWindow, button, coord);
        }
    }
    startMessageLoop();
    /*LOOP OUT*/
    mainWindow->textShower.clear();
    mainWindow->imageShower.clear();
    mainWindow->imageShower.refreshInstant();
    return chooseIndex;
}

void WindowUserInterface::stopProgram() {
    PostQuitMessage(0);
}




#endif