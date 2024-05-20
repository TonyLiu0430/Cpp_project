#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <string>
#include <list>

class GameObj {
private:
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
    Index operator-=(const Index &other) {
        i -= other.i;
        j -= other.j;
        return *this;
    }
    Index operator+=(const Index &other) {
        i += other.i;
        j += other.j;
        return *this;
    }
    bool operator==(const Index &other) const {
        return i == other.i && j == other.j;
    }
    bool operator!=(const Index &other) const {
        return i != other.i || j != other.j;
    }
};

enum class GameStatus {
    playing,
    win,
    lose
};

template<class UserInterface_Type>
class Game {
    UserInterface_Type ui;
public:
    std::list<std::vector<Index>> moveRecord;
    std::vector<std::vector<GameObj>> board;
    GameObj& getGameObj(const Index &index);
    Index player;
    GameStatus status = GameStatus::playing;
    void retToPrev();
    void start();
    void loadBoard(std::string filename);
    void playMove(const Index &to);
    bool isWin();
    bool isLose();
    bool isSafe(const Index &index);
    bool isSafe(int i, int j);
};

class QuitGameException : public std::exception {
public:
    virtual const char* what() const throw() override {
        return "Quit Game";
    }
};
