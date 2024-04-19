#pragma once
#include <vector>
#include "Poker.h"

class Player {
    int money = 10000;
public:
    int prevRoundGetMoney;
    std::vector<Poker> cards;
    int getPoint() const;
    int getMoney() const;
    int countA() const;
    void gamble(int stake, int &dealerGetMoney, bool init = true);
    class Status {
    public:
        int betMultiple = 0;
        std::string message;
        Status(std::string message = "", int betMultiple = 0);
    };

    void reset();
    /*       RESET        */
    int bet;
    Status status;
    /*       RESET        */
    void check();
    void stop();
    bool eliminate();
};

class Stop{
};
