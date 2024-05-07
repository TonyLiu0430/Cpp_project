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
    showBoard(game->board);
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

void WindowUserInterface::showBoard(const std::vector<std::vector<GameObj>> &board) {
    mainWindow->imageShower.insertImage(imageManager.getImage("playing"), {0, 0});
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
    mainWindow->imageShower.insertImage("win", imageManager.getImage("win"), {Image::statusLen + 300, Image::statusWidth + 300});
    mainWindow->imageShower.refreshInstant();
}

int WindowUserInterface::boardChoose(const std::vector<std::string> &boardList) {
    int chooseIndex = 0;
    mainWindow->imageShower.insertImage(imageManager.getImage("chooseLevel"), {0, 0});
    mainWindow->imageShower.refreshArea({0, 0, imageManager.getImage("chooseLevel")->length, imageManager.getImage("chooseLevel")->width});
    for(int i = 0; i < boardList.size(); i++) {
        if(i + 1 > 4) {
            cerr << "尚未支援超過4個關卡" << endl;
        }
        /*******BUGGGGG*/
        ButtonLike button("mission" + to_string(i + 1), [=, &chooseIndex](){
            cerr << "mission" + to_string(i + 1) << " clicked\n";
            try{
                chooseIndex = i;
                MainProgram::stopMessageLoop();
            } catch (...) {
                system("pause");
                cerr << "INSIDE";
                std::rethrow_exception(std::current_exception());
            }
            
        });
        mainWindow->insertButtonLike(button, {imageManager.getImage("chooseLevel")->length + 50 + i * 90, imageManager.getImage("chooseLevel")->width + 50});
    }
    try{
        MainProgram::startMessageLoop();
    } catch (std::bad_function_call &e) {
        cerr << "------------------------\n";
        Point p = {imageManager.getImage("chooseLevel")->length + 50 + 3 * 90, imageManager.getImage("chooseLevel")->width + 50};
        cerr << "===" << p.x << endl;
        cerr << std::dec;
        cerr << "msgCBs:\n";
        for(auto &[x, y] : mainWindow->messageCBs) {
            cerr << x << boolalpha << (y.target_type() == typeid(void)) << endl;
        }
        cerr << "msgCBs_noArgs:\n";
        for(auto &[x, y] : mainWindow->messageCBs_noArgs) {
            cerr << x << boolalpha << (y.target_type() == typeid(void))<< endl;
        }
        cerr << "mouse in:\n";
        for(auto &[x, y] : mainWindow->mouseProcesser.moveIn.cBs) {
            cerr << x.x << boolalpha << (y.target_type() == typeid(void))<< endl;
        }
        cerr << "mouse out:\n";
        for(auto &[x, y] : mainWindow->mouseProcesser.moveOut.cBs) {
            cerr << x.x << boolalpha << (y.target_type() == typeid(void))<< endl;
        }
        cerr << "mouse click:\n";
        for(auto &[x, y] : mainWindow->mouseProcesser.click.cBs) {
            cerr << x.x << boolalpha << (y.target_type() == typeid(void))<< endl;
        }

        cerr << "------------------------";
        system("pause");
        cerr << "OUTSIDE" << endl;
        cerr << e.what() << endl;
        std::rethrow_exception(std::current_exception());
    }
    //MainProgram::startMessageLoop();
    /*LOOP OUT*/
    mainWindow->imageShower.clear();
    mainWindow->imageShower.refreshInstant();
    return chooseIndex;
}




#endif