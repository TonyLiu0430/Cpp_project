#include "Game.h"
#include "UserInterface.hpp"
#include "ConsoleUserInterface.h"
#include "WindowUserInterface.h"
#include <bits/stdc++.h>
using std::cout;
using namespace std;

template<class T>
void Game<T>::start() {
    ui.showStart();
    loadBoard("resource/missions/mission3.txt");
    ui.start();
    ui.showBoard(board);
    ui.startMessageLoop();
    ui.end();
}

template<class T>
void Game<T>::loadBoard(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        throw Exception(filename + "Map file not found");
    }
    int n, m;
    file >> n >> m;
    board.resize(n, std::vector<char>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            file >> board[i][j];
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
void Game<T>::move(const Index &from, const Index &direction) {
    if(abs(direction.i) + abs(direction.j) != 1) {
        throw Exception("Invalid direction");
    }
    if(!isSafe(from) || !isSafe(from + direction)) {
        throw InvalidMoveException();
    }
    if(isWall(from + direction)) {
        throw InvalidMoveException();
    }
    if(isRoad(from + direction)) {
        swapGameObj(board[from.i][from.j], board[from.i + direction.i][from.j + direction.j]);
        return;
    }
    if(isBox(from + direction)) {
        move(from + direction, direction);
    }
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
bool Game<T>::isRoad(const Index &index) {
    return board[index.i][index.j] == road;
}


template<class T>
bool Game<T>::isWall(const Index &index) {
    return board[index.i][index.j] == wall;
}


template<class T>
bool Game<T>::isBox(const Index &index) {
    return board[index.i][index.j] & box != 0;
}


template<class T>
bool Game<T>::isWin() {
    for (int i = 0; i < board.size(); i++ ) {
        for (int j = 0; j < board[i].size(); j++ ) {
            if(isCheckPoint({i, j}) && !isBox({i, j})) {
                return false;
            }
        }
    }
    return true;
}

template<class T>
void Game<T>::swapGameObj(char &a, char &b) {
    char aIsCheck = a & checkPoint;
    char bIsCheck = b & checkPoint;
    a ^= b;
    b ^= a;
    a ^= b;
    a ^= aIsCheck;
    b ^= bIsCheck;
}

template<class T>
bool Game<T>::isCheckPoint(const Index &index) {
    return board[index.i][index.j] & checkPoint != 0;
}



template class Game<WindowUserInterface>;
template class Game<ConsoleUserInterface>;
