#pragma once
#include "UserInterface.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include <utility>

class GameObj {
    int data;
    inline const static int checkPoint = 0b10000;
    inline const static int box = 0b01000;
    inline const static int wall = 0b00100;
    inline const static int player = 0b00010;
    inline const static int road = 0;
public:
    GameObj(char input = 0);
    bool isRoad() const;
    bool isWall() const;
    bool isBox() const;
    bool isCheckPoint() const;
    bool isPlayer() const;
    friend void swapGameObj(GameObj &a, GameObj &b);
};

template<class UserInterface_Type>
class Game {
    UserInterface_Type ui;
    std::vector<std::vector<GameObj>> board;
    struct Index{
        int i;
        int j;
        Index(int i, int j): i(j), j(j) {}
        Index(const std::pair<int, int> &p): i(p.first), j(p.second) {}
        Index() = default;
        Index operator+(const Index &other) const {
            return Index(i + other.i, j + other.j);
        }
    };
    GameObj& getGameObj(const Index &index);
    Index player;
    class InvalidMoveException: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Invalid move";
        }
    };
public:
    void start();
    void loadBoard(std::string filename);
    void move(const Index &from, const Index &to);
    bool isWin();
    /*TODO*/ bool isLost() { return false; };
    bool isSafe(const Index &index);
    bool isSafe(int i, int j);
};



//void swapGameObj(GameObj &a, GameObj &b);
