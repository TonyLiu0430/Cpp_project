#include "ConsoleUserInterface.h"

#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "util.h"
using std::cout;
using namespace std;

static char getChar() {
    #ifdef _WIN32
    char ch = _getch();
    if(ch == '\r') {
        return '\n';
    }
    return ch;
    #else
    termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = std::getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
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
    insertCharCallback('r', [=](){
        game->retToPrev();
    });
    insertCharCallback('q', [=](){
        showLose();
        stopMessageLoop();
    });
    showBoard(game->board);
    startMessageLoop();
    /*Loop out*/
    charCallback.clear();
    //clearScreen();
}

void ConsoleUserInterface::insertCharCallback(char c, function<void()> callback) {
    charCallback[c] = callback;
    charCallback[c ^ 0b100000] = callback;
}

void ConsoleUserInterface::showStart() {
    cout << "歡迎進入倉庫番遊戲" << endl;
    cout << endl;
    cout << "按任意鍵繼續..." << endl;
    getChar();
    clearScreen();
    cout << "操作說明:" << endl;
    cout << "使用wasd控制人物移動" << endl;
    cout << "r: 上一步" << endl;
    cout << "q: 投降" << endl;
    cout << endl;
    cout << "按任意鍵繼續..." << endl;
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

int ConsoleUserInterface::boardChoose(const vector<fs::path> &boardList) {
    cout << "請選擇地圖" << endl;
    for(int i = 0; i < boardList.size(); i++) {
        cout << i + 1 << ": " << boardList[i].filename().string() << endl;
    }
    int choose;
    cin >> choose;
    while(choose < 1 || choose > boardList.size()) {
        cout << "請輸入範圍內數字 : " << 1 << " ~ " << boardList.size() << endl;
        cin >> choose;
    }
    return choose - 1;
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
                    cout << "i";
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

void ConsoleUserInterface::end(Game<ConsoleUserInterface> *game) {
    cout << "\n\n";
    if(game->status == GameStatus::win) {
        showWin();
    }
    else if(game->status == GameStatus::lose) {
        showLose();
    }
    cout << "遊戲結束" << endl;
    cout << endl;
    cout << "按任意鍵繼續..." << endl;
    getChar();
    clearScreen();
}

void ConsoleUserInterface::showWin() {
    cout << "恭喜過關" << endl;
    cout << endl;
    cout << "按任意鍵繼續..." << endl;
    getChar();
    clearScreen();
}

void ConsoleUserInterface::showLose() {
    cout << "失敗" << endl;
    cout << endl;
    cout << "按任意鍵繼續..." << endl;
    getChar();
    clearScreen();
}