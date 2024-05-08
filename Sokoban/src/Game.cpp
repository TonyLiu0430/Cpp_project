#include "Game.h"
#include "UserInterface.hpp"
#include "ConsoleUserInterface.h"
#include "WindowUserInterface.h"
#include <bits/stdc++.h>
using std::cout;
using namespace std;

template<class T>
void Game<T>::start() {
    try {
        ui.showStart();
        vector<string> boardList = getBoardList();
        while(true) {
            int chooseBoardIndex = ui.boardChoose(boardList);
            string chooseBoard = boardList[chooseBoardIndex];
            loadBoard(chooseBoard);
            ui.start(this);
            ui.end();
        }
    } catch (QuitGameException &e) {
        return;
    }
}

template<class T>
void Game<T>::loadBoard(std::string filename) {
    cerr << "loadBoard start\n";
    std::ifstream file(filename);
    if (!file) {
        throw Exception(filename + "Map file not found");
    }
    int n, m;
    file >> n >> m;
    cerr << n << m;
    board.clear();
    board.resize(n, std::vector<GameObj>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            char temp;
            file >> temp;
            board[i][j] = GameObj(temp);
            if(board[i][j].isPlayer()) {
                player = {i, j};
            }
        }
    }
    cerr << "loadBoard success\n";
    /*
    /(牆)
    -(道路)
    0(人物當前位置)
    1(箱子)
    2(終點)
    */
}

template<class T>
vector<string> Game<T>::getBoardList() {
    filesystem::path dirPath = "missions";
    if(!filesystem::exists(dirPath)) {
        throw Exception("missions folder not found");
    }
    filesystem::directory_entry dir(dirPath);
    if(!dir.is_directory()) {
        throw Exception("missions is not a directory");
    }
    vector<string> boardList;
    filesystem::directory_iterator dirIt(dirPath);
    for(auto &p: dirIt) {
        if(p.is_regular_file()) {
            //cout << ">>>" << p.path().string() << endl;
            boardList.push_back(p.path().string());
        }
    }
    return boardList;
}

template<class T>
void Game<T>::move(const Index &from, const Index &direction) {
    if(abs(direction.i) + abs(direction.j) != 1) {
        throw Exception("Invalid direction");
    }
    if(!isSafe(from) || !isSafe(from + direction)) {
        throw InvalidMoveException();
    }
    if(getGameObj(from + direction).isWall()) {
        throw InvalidMoveException();
    }
    if(getGameObj(from).isPlayer() && getGameObj(from + direction).isBox()) {
        move(from + direction, direction);
        swapGameObj(board[from.i][from.j], board[from.i + direction.i][from.j + direction.j]);
        return;
    }
    if(getGameObj(from + direction).isRoad() || getGameObj(from + direction).isCheckPoint()) {
        swapGameObj(board[from.i][from.j], board[from.i + direction.i][from.j + direction.j]);
        return;
    }
    throw InvalidMoveException();
}

template<class T>
GameObj& Game<T>::getGameObj(const Index &index) {
    if(!isSafe(index)) {
        throw Exception("index ERROR");
    }
    return board[index.i][index.j];
}

template<class T>
bool Game<T>::isSafe(const Index &index) {
    return isSafe(index.i, index.j);
}

template<class T>
bool Game<T>::isSafe(int i, int j) {
    return i >= 0 && i < board.size() && j >= 0 && j < board[i].size();
}



template<class T>
bool Game<T>::isWin() {
    for (int i = 0; i < board.size(); i++ ) {
        for (int j = 0; j < board[i].size(); j++ ) {
            if(board[i][j].isCheckPoint() && !board[i][j].isBox()) {
                return false;
            }
        }
    }
    return true;
}

template<class T>
bool Game<T>::isLose() {
    int unsolvedCheckPointCnt = 0;
    int unsolvedBoxCnt = 0;
    int stuckBoxCnt = 0;
    for (int i = 0; i < board.size(); i++ ) {
        for (int j = 0; j < board[i].size(); j++ ) {
            if(board[i][j].isCheckPoint() && board[i][j].isBox()) {
                continue;
            }
            if(board[i][j].isCheckPoint()) {
                unsolvedCheckPointCnt++;
            }
            if(board[i][j].isBox()) {
                unsolvedBoxCnt++;
                bool i_stuck = (!isSafe(i - 1, j) || board[i - 1][j].isWall()) || (!isSafe(i + 1, j) || board[i + 1][j].isWall());
                bool j_stuck = (!isSafe(i, j - 1) || board[i][j - 1].isWall()) || (!isSafe(i, j + 1) || board[i][j + 1].isWall());
                if(i_stuck && j_stuck) {
                    stuckBoxCnt++;
                }
            }
        }
    }
    int freeBoxCnt = unsolvedBoxCnt - stuckBoxCnt;
    return  freeBoxCnt < unsolvedCheckPointCnt;
}

template<class T>
void Game<T>::playMove(const Index &to) {
    try {
        move(player, to);
        player += to;
    } catch (InvalidMoveException &e) {
        return;
    }
    ui.showBoard(board);
    if(isWin()) {
        ui.showWin();
        ui.stopMessageLoop();
    }
    if(isLose()) {
        ui.showLose();
        ui.stopMessageLoop();
    }
}


GameObj::GameObj(char input) {
    /*
    /(牆)
    -(道路)
    0(人物當前位置)
    1(箱子)
    2(終點)
    */
    if(input == '/') {
        data = wall;
    } 
    else if(input == '-') {
        data = road;
    } 
    else if(input == '0') {
        data = player;
    } 
    else if(input == '1') {
        data = box;
    } 
    else if(input == '2') {
        data = checkPoint;
    } 
    else {
        throw Exception("Invalid input");
    }
}

bool GameObj::isRoad() const {
    return data == road;
}

bool GameObj::isWall() const {
    return data == wall;
}

bool GameObj::isBox() const {
    return (data & box) != 0;
}

bool GameObj::isCheckPoint() const {
    return (data & checkPoint) != 0;
}

bool GameObj::isPlayer() const {
    return (data & player) != 0;
}


void swapGameObj(GameObj &a, GameObj &b) {
    int aCpFlag = a.data & GameObj::checkPoint;
    int bCpFlag = b.data & GameObj::checkPoint;
    a.data &= GameObj::checkPoint - 1;
    b.data &= GameObj::checkPoint - 1;
    std::swap(a.data, b.data);
    a.data |= aCpFlag;
    b.data |= bCpFlag;
}


#ifdef AWG
template class Game<WindowUserInterface>;
#endif
//template class Game<ConsoleUserInterface>;
