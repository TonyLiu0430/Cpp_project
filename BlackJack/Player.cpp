#include "Player.h"
#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

int Player::getMoney() const {
    return money;
}

int Player::getPoint() const {
    int res = 0;
    int cntA = 0;
    for (const auto &card : cards) {
        if(card.num == 1) {
            cntA += 1;
            res += 1;
        }
        else if(card.num < 10) {
            res += card.num;
        }
        else {
            res += 10;
        }
    }
    for (int i = 0; i < cntA; i++ ) {
        if(res + 10 <= 21) {
            res += 10;
        }
    }
    return res;
}

int Player::countA() const {
    int cntA = 0;
    for (const auto &card : cards) {
        if(card.num == 1) {
            cntA += 1;
        }
    }
    return cntA;
}

void Player::reset() {
    bet = 0;
    status = Status();
}

Player::Status::Status(string message, int betMultiple) :  message(message), betMultiple(betMultiple) {};

void Player::check() {
    /*爆牌*/
    if(getPoint() > 21) {
        status = Status("爆牌", 0);
        throw Stop();
    }

    /*checkStraight*/
    if(cards.size() == 3) {
        set<int> s{cards[0].num, cards[1].num, cards[2].num};
        if(s == set<int>({6, 7, 8})) {
            status = Status("獲得順子牌型", 3);
            throw Stop();
        }
    }

    /*過五關*/
    if(cards.size() >= 5) {
        status = Status("過五關", 3);
        throw Stop();
    }

    /*21點*/
    if(getPoint() == 21) {
        status = Status("獲得21點", 2);
        throw Stop();
    }
}

void Player::stop() {
    if(getPoint() != 21) {
        status = Status("已停牌", 0);
    }
    else {
        status = Status("獲得21點", 2);
    }
    throw Stop();
}

bool Player::eliminate() {
    return money < MinimumBet;
}

void Player::gamble(int stake, int &dealerGetMoney, bool init) {
    money += stake;
    if(init) {
        prevRoundGetMoney = stake;
    }
    else {
        prevRoundGetMoney += stake;
    }
    dealerGetMoney -= stake;
}