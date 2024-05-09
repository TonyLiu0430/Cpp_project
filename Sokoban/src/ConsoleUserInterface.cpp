#include "ConsoleUserInterface.h"

#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#endif

#include "util.h"
using std::cout;
using namespace std;

static char getChar() {
    #ifdef _WIN32
    return _getch();
    #else
    string str;
    cin >> str;
    return str[0];
    #endif
}

static void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}


void ConsoleUserInterface::startPlay(Game<ConsoleUserInterface> *game) {
    insertCharCallback('w', [=]() {
        game->playMove({-1, 0});
    });
    insertCharCallback('a', [=](){ 
        game->playMove({0, -1});
    });
    insertCharCallback('s', [=]() {
        game->playMove({1, 0});
    });
    insertCharCallback('d', [=]() {
        game->playMove({0, 1});
    });
    showBoard(game->board);
    startMessageLoop();

    clearScreen();
}

void ConsoleUserInterface::insertCharCallback(char c, function<void()> callback) {
    charCallback[c] = callback;
    charCallback[c ^ 0b100000] = callback;
}

void ConsoleUserInterface::showStart() {
    cout << "歡迎進入倉庫番遊戲" << endl;
    getChar();
    /*TODO*/
}

void ConsoleUserInterface::startMessageLoop() {
    MsgLoopRunning = true;
    while(MsgLoopRunning) {
        char c = getChar();
        if(charCallback.find(c) != charCallback.end()) {
            charCallback[c]();
        }
    }

}

void ConsoleUserInterface::stopMessageLoop() {
    MsgLoopRunning = false;
}

int ConsoleUserInterface::boardChoose(const vector<std::filesystem::path> &boardList) {
    cout << "請選擇地圖" << endl;
    for(int i = 0; i < boardList.size(); i++) {
        cout << i << ": " << boardList[i] << endl;
    }
    int choose;
    cin >> choose;
    return choose;
}
/*
    /(牆)
    -(道路)
    0(人物當前位置)
    1(箱子)
    2(終點)
    */

void ConsoleUserInterface::showBoard(const vector<vector<GameObj>> &board) {
    clearScreen();
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            if(board[i][j].isWall()) {
                cout << "/";
            } 
            else if(board[i][j].isRoad()) {
                cout << "-";
            }
            else if(board[i][j].isBox()) {
                if(board[i][j].isCheckPoint()) {
                    cout << "3";
                }
                else {
                    cout << "1";
                }
            }
            else if(board[i][j].isPlayer()) {
                if(board[i][j].isCheckPoint()) {
                    cout << "0";
                }
                else {
                    cout << "0";
                }
            }
            else if(board[i][j].isCheckPoint()) {
                cout << "2";
            }
        }
        cout << endl;
    }
}

void ConsoleUserInterface::end() {
    cout << "遊戲結束" << endl;
    getChar();
}

void ConsoleUserInterface::showWin() {
    cout << "恭喜過關" << endl;
    getChar();
}

void ConsoleUserInterface::showLose() {
    cout << "失敗" << endl;
    getChar();
}