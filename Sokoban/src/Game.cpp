#include "Game.h"
#include "util.h"
#include "ConsoleUserInterface.h"
#include "WindowUserInterface.h"
#include "filesystemCompatible.h"
#include <bits/stdc++.h>
using std::cout;
using namespace std;

template<class T>
void Game<T>::start() {
    ui.showStart();
    vector<fs::path> boardList = getBoardList("missions");
    while(true) {
        moveRecord.clear();
        int chooseBoardIndex = ui.boardChoose(boardList);
        string chooseBoard = boardList[chooseBoardIndex].string();
        loadBoard(chooseBoard);
        while(status == GameStatus::playing) {
            ui.startPlay(this);
            ui.end(this);
        }
        status = GameStatus::playing;
    }
}

template<class T>
void Game<T>::loadBoard(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        throw UnexpectedException(filename + "Map file not found");
    }
    int n, m;
    file >> n >> m;
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
    /*
    /(牆)
    -(道路)
    0(人物當前位置)
    1(箱子)
    2(終點)
    */
}

template<class T>
GameObj& Game<T>::getGameObj(const Index &index) {
    if(!isSafe(index)) {
        throw UnexpectedException("index ERROR");
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
    if(abs(to.i) + abs(to.j) != 1) {
        return;
    }
    if(!isSafe(player) || !isSafe(player + to)) {
        return;
    }
    if(getGameObj(player + to).isWall()) {
        return;
    }
    if(getGameObj(player).isPlayer() && getGameObj(player + to).isBox()) {
        if(!isSafe(player + to + to) || getGameObj(player + to + to).isWall() || getGameObj(player + to + to).isBox()) {
            return;
        }
        swapGameObj(getGameObj(player + to), getGameObj(player + to + to));
        swapGameObj(getGameObj(player), getGameObj(player + to));
        moveRecord.push_back({to, player, player + to});
        player += to;
    }
    else if(getGameObj(player + to).isRoad() || getGameObj(player + to).isCheckPoint()) {
        swapGameObj(getGameObj(player), getGameObj(player + to));
        moveRecord.push_back({to, player});
        player += to;
    }
    else {
        return;
    }
    ui.showBoard(board);
    if(isWin()) {
        status = GameStatus::win;
        ui.stopMessageLoop();
    }
    if(isLose()) {
        status = GameStatus::lose;
        ui.stopMessageLoop();
    }
}

template<class T>
void Game<T>::retToPrev() {
    if(moveRecord.empty()) {
        return;
    }
    vector<Index> lastMove = moveRecord.back();
    moveRecord.pop_back();
    if(lastMove.size() == 0) {
        return;
    }
    Index to = lastMove[0];
    Index ogPlayer = lastMove[1];
    Index ogBox = lastMove[2];
    if(ogPlayer + to != player) {
        throw UnexpectedException("Player position error");
    }

    if(lastMove.size() == 2) {
        swapGameObj(getGameObj(ogPlayer), getGameObj(ogPlayer + to));
    }
    else {
        swapGameObj(getGameObj(ogPlayer), getGameObj(ogPlayer + to));
        swapGameObj(getGameObj(ogPlayer + to), getGameObj(ogBox + lastMove[0]));
    }
    player = ogPlayer;
    ui.showBoard(board);
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
    else if(input == '3') {
        data = box | checkPoint;
    }
    else {
        throw UnexpectedException("Invalid input");
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

bool GameObj::operator!=(const GameObj &other) const {
    return data != other.data;
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
template class Game<ConsoleUserInterface>;
