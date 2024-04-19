#include "Game.h"
#include "util.h"
#include "Printer.h"
#include <bits/stdc++.h>

using namespace std;

//int PLAYER_ID;

Game::Game(int roundNum, int playerNum) : roundNum(roundNum), playerNum(playerNum), players(playerNum){};

int Game::start() {
    for (int i = 0; i < roundNum; i++ ) {
        int dealerId;
        if(i == 0) {
            dealerId = random(playerNum);
        }
        else {
            dealerId = getLargestPrevRoundMoneyPlayerId(); /**上局所獲得金錢(賭注被沒收則為負)最高者擔任*/ // getLowestMoneyPlayerId();
        }
        Round round(*this, dealerId, players);
        round.run();
    }
    showGameEndInfo();
    return 0;
}

void Game::showGameEndInfo() {
    clearScreen();
    printer.insertLineInstant("遊戲結束資訊");
    printer.insertLineInstant("");
    vector<pair<Player, int>> rankedPlayers;
    for (int id = 0; id < playerNum; id++) {
        rankedPlayers.push_back({players[id], id});
    }
    sort(rankedPlayers.begin(), rankedPlayers.end(),
         [](const pair<Player, int> &a, const pair<Player, int> &b){
             return a.first.getMoney() > b.first.getMoney();
         });
    for (int i = 0; i < playerNum; i++) {
        auto &player = rankedPlayers[i].first;
        auto &id = rankedPlayers[i].second;
        string output;
        output += "[玩家"s + to_string(id + 1);
        if(isUser(id)) {
            output += " #真人玩家";
            output += to_string(getUserSer(id));
            output += "# ";
        }
        else {
            output += " 電腦玩家 ";
        }
        output += "]\t";
        output += " 最終持有 " + to_string(player.getMoney()) + " 元\t" + " 獲得 第" + to_string(i + 1) + " 名";
        printer.insertLineInstant(output);
    }
    printer.insertLine("");
    printer.insertLineInstant("");
    pressEnterContinue();
}

int Game::getLargestPrevRoundMoneyPlayerId() {
    int resId = 0;
    for (int id = 1; id < playerNum; id++ ) {
        if(players[id].eliminate()) {
            continue;
        }
        if(players[id].prevRoundGetMoney > players[resId].prevRoundGetMoney) {
            resId = id;
        }
        else if(players[id].prevRoundGetMoney == players[resId].prevRoundGetMoney) {
            if(players[id].getMoney() < players[resId].getMoney()) {
                resId = id;
            }
        }
    }
    return resId;
}

std::map<int, int>& Game::getUsers() {
    return users;
}

int Game::getUserSer(int id) {
    assert(isUser(id));
    return users[id];
}

bool Game::isUser(int id) {
    return users.find(id) != users.end();
}

