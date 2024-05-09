#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <string>
#include <filesystem>

class GameObj {
public:/*DEBUG*/
    int data;
    inline const static int checkPoint  = 0b10000;
    inline const static int box         = 0b01000;
    inline const static int wall        = 0b00100;
    inline const static int player      = 0b00010;
    inline const static int road        = 0;
public:
    GameObj(char input);
    GameObj(): data(0) {}
    bool isRoad() const;
    bool isWall() const;
    bool isBox() const;
    bool isCheckPoint() const;
    bool isPlayer() const;
    bool operator!=(const GameObj &other) const;
    friend void swapGameObj(GameObj &a, GameObj &b);
};

struct Index{
    int i;
    int j;
    Index(int i, int j): i(i), j(j) {}
    Index(const std::pair<int, int> &p): i(p.first), j(p.second) {}
    Index() = default;
    Index operator+(const Index &other) const {
        return Index(i + other.i, j + other.j);
    }
    Index operator+=(const Index &other) {
        i += other.i;
        j += other.j;
        return *this;
    }
};

template<class UserInterface_Type>
class Game {
    UserInterface_Type ui;
public:
    std::vector<std::vector<GameObj>> board;
    GameObj& getGameObj(const Index &index);
    Index player;
    class InvalidMoveException: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Invalid move";
        }
    };
    class QuitGameException: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Quit game";
        }
    };

    void start();
    void loadBoard(std::string filename);
    std::vector<std::filesystem::path> getBoardList();
    void move(const Index &from, const Index &to);
    void playMove(const Index &to);
    bool isWin();
    bool isLose();
    bool isSafe(const Index &index);
    bool isSafe(int i, int j);
};



//void swapGameObj(GameObj &a, GameObj &b);
