#pragma once
#include "UserInterface.hpp"
#include <memory>
#include <iostream>

template<class T>
class Game {
    UserInterface<T> ui;
    class GameObj: public UserInterface<T>::UI::GameObj_base {
    public:
        int x, y;
        virtual bool canMove() = 0;
    };
    class Player: public GameObj {
        virtual bool canMove() {
            return true;
        }
    };
    class Wall: public GameObj {
        virtual bool canMove() {
            return false;
        }
    };
    class land: public GameObj {
        virtual bool canMove() {
            return false;
        }
    };
    class Box: public GameObj {
        virtual bool canMove() {
            return true;
        }
    };
    std::vector<std::vector<GameObj*>> board;
    Player *player;
public:
    void start();
    void loadBoard(std::string filename);
};

