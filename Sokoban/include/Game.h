#pragma once
#include "UserInterface.hpp"
#include <memory>
#include <vector>
#include <iostream>

template<class UserInterface_Type>
class Game {
    UserInterface_Type ui;
    std::vector<std::vector<char>> board;
    struct Index{
        int i;
        int j;
        Index(int i, int j): i(j), i(j) {}
        Index(const pair<int, int> &p): i(p.first), j(p.second) {}
        Index() = default;
        Index operator+(const Index &other) {
            return Index(i + other.i, j + other.j);
        }
    };
    Index player;
    class InvalidMoveException: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Invalid move";
        }
    };
public:
    inline const static char checkPont = 0b10000;
    inline const static char box = 0b01000;
    inline const static char wall = 0b00100;
    inline const static char road = 0;
    void start();
    void loadBoard(std::string filename);
    void move(const Index &from, const Index &to);
    bool isWin();
    /*TODO*/bool isLost();
    bool isSafe(const Index &index);
    bool isSafe(int i, int j);
    bool isRoad(const Index &index);
    bool isWall(const Index &index);
    bool isBox(const Index &index);
    static void swapGameObj(char &a, char &b);
};

