#include "Game.h"
#include "Printer.h"
#include "util.h"
#include "AiStrategy.h"
#include "KeyboardGetter.h"
#include <bits/stdc++.h>
using namespace std;

Game::Round::Round(Game &game, int dealerId, std::vector<Player> &players) : game(game), dealerId(dealerId), playerNum(players.size()), players(players) {
    for(auto &player : players) {
        player.reset();
    }
};

void Game::Round::run() {
    setBet();
    init();
    for (int id = 0; id < playerNum; id++ ) {
        if(id == dealerId || players[id].eliminate()) {
            continue;
        }
        else if(game.isUser(id) == false) {
            aiPlay(players[id], id);
        }
        else if(game.isUser(id)) {
            play(players[id], id);
        }
    }
    if(game.isUser(dealerId)) {
        playDealer(players[dealerId]);
    }
    else {
        aiDealer(players[dealerId]);
    }
    //************/
    calculateEarned();
    showInfo(true, true);
    printer.insertLineInstant("回合結束玩家手牌資訊");
    printer.insertLineInstant("");
    pressEnterContinue();
    
    showRoundEndInfo();
}

void Game::Round::showRoundEndInfo() {
    clearScreen();
    printer.insertLine("回合結束資訊");
    printer.insertLine("");
    for (int id = 0; id < playerNum; id++ ) {
        string output;
        output += playerInfoStr(id);
        output += " 本回合";
        if(players[id].prevRoundGetMoney >= 0) {
            output += " 獲得 ";
        }
        else {
            output += " 失去 ";
        }
        output += to_string(abs(players[id].prevRoundGetMoney)) + " 元\t";
        output += " 目前持有 "s + to_string(players[id].getMoney()) + " 元 ";
        printer.insertLine(output);
    }
    printer.insertLine("");
    printer.insertLine("");
    printer.insertLine("");
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
        output += "第" + to_string(i + 1) + " 名是 ";
        output += playerInfoStr(id);
        output += " 目前持有 " + to_string(player.getMoney()) + " 元";
        printer.insertLine(output);
    }
    printer.insertLine("");
    printer.insertLine("");
    printer.print();
    pressEnterContinue();
    clearScreen();
}

void Game::Round::calculateEarned() {
    int dealerGetMoney = 0;
    for(int id = 0; id < playerNum; id++ ) {
        if(id != dealerId) {

            /* UGLY !!! MAYBE REFEACTOR*/
            if(players[dealerId].status.betMultiple == -1 /*投降*/) {
                players[id].gamble(-players[id].bet, dealerGetMoney);
                continue;
            }
            else if(players[dealerId].getPoint() > 21 /*莊家爆牌*/) {
                if(players[id].status.betMultiple > 0) {
                    players[id].gamble(players[id].bet * (players[id].status.betMultiple - 1), dealerGetMoney);
                }
                else {
                    if(players[id].getPoint() > 21) {
                        players[id].gamble(0, dealerGetMoney);
                    }
                    else {
                        players[id].gamble(players[id].bet, dealerGetMoney);
                    }
                }
            }
            else /*莊家未爆牌*/{
                if(players[id].status.betMultiple == 3 /*特殊排型*/) {
                    players[id].gamble(players[id].bet * (players[id].status.betMultiple - 1), dealerGetMoney);
                }
                else if(players[id].status.betMultiple == 2 && players[dealerId].getPoint() == 21/*同時21點*/) {
                    players[id].gamble(0, dealerGetMoney); /*平手*/
                }
                else {
                    if(players[id].getPoint() > 21) {
                        players[id].gamble(-players[id].bet, dealerGetMoney);
                    }
                    else {
                        if(players[id].getPoint() <= players[dealerId].getPoint()){
                            players[id].gamble(-players[id].bet, dealerGetMoney);
                        }
                        else {
                            players[id].gamble(players[id].bet, dealerGetMoney);
                        }
                    }
                }
            }
        }
    }
    for(const int id : insuranceIds) {
        if(players[dealerId].cards.size() == 2 && players[dealerId].getPoint() == 21) {
            players[id].gamble(players[id].bet / 2, dealerGetMoney, false);
        }
        else {
            players[id].gamble(-(players[id].bet / 2), dealerGetMoney, false);
        }
    }
    players[dealerId].gamble(dealerGetMoney, dealerGetMoney);
}

void Game::Round::play(Player &player, int id) {
    try{
        insure(player, id);
        while(true) {
            showInfo(id == dealerId);
            printer.insertLineInstant(playerInfoStr(id) + " 目前點數為 " + to_string(player.getPoint()));
            printer.insertLine("");
            player.check(); /*check 21 點*/
            KeyboardGetter keyboardGetter;
            if(id != dealerId && player.cards.size() == 2) { //投降
                keyboardGetter.insertCharEvent('s', "\t投降 按下 \'s\'\n", [&player, this]() {
                                                    surrender(player);
                                                });
            }
            keyboardGetter.insertCharEvent('y', "\t要牌 按下 \'y\'", [&player, this]() {
                                                distributeCard(player);
                                                player.check(); 
                                            });
            keyboardGetter.insertCharEvent('n', "\t停牌 按下 \'n\'", [&player, this]() {
                                                player.stop();
                                            });
            if(id != dealerId && players[id].getMoney() >= players[id].bet * 2) {
                keyboardGetter.insertCharEvent('d', "\t雙倍下注 按下 \'d\'", [&player, this]() {
                                                    doubleDown(player);
                                                    player.check();/*throw*/
                                                    player.stop();
                                                });
            }
            keyboardGetter.get();
        }
    }catch(Stop e) {
        showInfo(id == dealerId);
        printer.insertLineInstant(playerStatusInfoStr(id));
        pressEnterContinue();
    }
}

void Game::Round::playDealer(Player &player) {
    try{
        while(player.getPoint() < 17) {
            showInfo(true);
            string output;
            output += playerInfoStr(dealerId);
            output += "點數未達17自動補牌";
            printer.insertLineInstant(output);
            hit(player);
            player.check();
            pressEnterContinue();
        }
        play(player, dealerId);
    }catch(Stop e) {
        pressEnterContinue();
        showInfo(true);
        printer.insertLineInstant(playerStatusInfoStr(dealerId));
        pressEnterContinue();
    }
}

void Game::Round::aiPlay(Player &player, int id) {
    try{
        while(true) {
            showInfo();
            string output;
            output += playerInfoStr(id);
            output += "AI操作中";
            printer.insertLineInstant(output);
            string strategy;
            if(player.countA() == 0 || player.cards.size() >= 3) {
                strategy = aiStrategy.hardStrategy(players[dealerId].cards[0].num, player.getPoint());
            }
            else {
                int lastCardPoint = (player.cards[0].num == 1 ? player.cards[1].num : player.cards[0].num);
                strategy = aiStrategy.softStrategy(players[dealerId].cards[0].num, lastCardPoint);
            }
            if(strategy == "H") {
                hit(player);
            }
            else if(strategy == "S") {
                player.stop();
            }
            else if(strategy == "D") {
                if(player.getMoney() >= player.bet * 2) {
                    doubleDown(player);
                    player.stop();  
                }
                else {
                    hit(player);
                }
            }
            else if(strategy == "DS") {
                if(player.getMoney() >= player.bet * 2) {
                    doubleDown(player);
                    player.stop();  
                }
                else {
                    player.stop();
                }
            }
            else if(strategy == "RH") {
                if(player.cards.size() == 2) {
                    surrender(player);
                }
                hit(player);
            }
            else if(strategy == "RS") {
                if(player.cards.size() == 2) {
                    surrender(player);
                }
                player.stop();
            }
            else if(strategy == "W") {

            }
            else {
                player.stop();
            }
            player.check();
            pressEnterContinue();
        }
    }catch(Stop e) {
        pressEnterContinue();
        showInfo();
        printer.insertLineInstant(playerStatusInfoStr(id));
        pressEnterContinue();
    }
}

void Game::Round::aiDealer(Player &player) {
    try{
        while(player.getPoint() < 17) {
            showInfo(true);
            string output;
            output += playerInfoStr(dealerId);
            output += "點數未達17自動補牌";
            printer.insertLineInstant(output);
            hit(player);
            if(player.getPoint() > 21) {
                player.status = Player::Status("爆牌", 0);
                throw Stop();
            }
            pressEnterContinue();
        }
        player.stop();
    }catch(Stop e) {
        showInfo(true);
        printer.insertLineInstant(playerStatusInfoStr(dealerId));
        pressEnterContinue();
    }
}

void Game::Round::hit(Player &player) {
    distributeCard(player);
}

void Game::Round::surrender(Player &player) {
    player.bet /= 2;
    player.status = Player::Status("已投降", -1);
    throw Stop();
}

void Game::Round::doubleDown(Player &player) {
    //assert(player.money >= player.bet);
    player.bet *= 2;
    distributeCard(player);
}

void Game::Round::distributeCard(Player &player) {
    player.cards.push_back(deck.distributeCard());
}

void Game::Round::init() {
    /*reset*/
    for (auto &player : players) {
        player.cards.clear();
    }

    for (auto &player : players) {
        if(player.eliminate() == false) {
            distributeCard(player);
        }
        /********************************************************/
        /********************************************************/
    }
    for (auto &player : players) {
        if(player.eliminate() == false) {
            distributeCard(player);
        }
        /********************************************************/
        /********************************************************/
    }
}

void Game::Round::insure(Player &player, int id) {
    if(players[dealerId].cards[0].num != 1 || players[dealerId].cards.size() != 2 || id == dealerId) {
        return;
    }
    else if(game.isUser(id) == false) {
        /*1% 機率 下保險*/
        int randomNum = random(100);
        if(randomNum == 0) {
            insuranceIds.push_back(id);
        }
    }
    else if(game.isUser(id)) {
        showInfo();
        if(2 * player.getMoney() > 3 * player.bet) {
            KeyboardGetter keyboardGetter;
            keyboardGetter.insertCharEvent('y'
                                            ,{"莊家明牌為 A"
                                             ,"\t你要購買保險嗎"
                                             ,"要\t按下 \'y\'"}
                                            , [id, this]() {
                                                insuranceIds.push_back(id);
                                                printer.insertLineInstant("已購買保險");
                                            });
            keyboardGetter.insertCharEvent('n'
                                            ,"不要\t按下 \'n\'"
                                            , [id, this]() {
                                                insuranceIds.push_back(id);
                                                printer.insertLineInstant("放棄購買保險");
                                            });
            keyboardGetter.get();
        }
        else {
            printer.insertLine("莊家明牌為 A");
            printer.insertLineInstant("你的錢不夠 無法購買保險");
        }
        pressEnterContinue();
    }
}

void Game::Round::showInfo(bool showDealer, bool showEarned) {
    printer.reset();
    printer.insertLine("");/************/
    printer.insertLine(playerInfoStr(dealerId) + "的手牌資訊");
    if(showDealer == false) {
        printer.insertDealerPokerCards(players[dealerId].cards /*暗牌*/);
    }
    else {
        string output = "莊家";
        output += " 點數為: " + to_string(players[dealerId].getPoint());
        output += " "s + players[dealerId].status.message;
        if(showEarned) {
            output += " 本回合";
            if(players[dealerId].prevRoundGetMoney >= 0) {
                output += " 獲得 ";
            }
            else {
                output += " 失去 ";
            }
            output += to_string(abs(players[dealerId].prevRoundGetMoney)) + " 元 ";
        }
        printer.insertLine(output);
        printer.insertPokerCards(players[dealerId].cards);
    }
    printer.insertSeparateLine();
    printer.insertSeparateLine();
    printer.insertSeparateLine();
    for (int id = 0; id < playerNum; id++ ) {
        const int observedId = id;
        int observedSerNum = observedId + 1;
        if(players[observedId].eliminate() == true) {
            printer.insertLine(playerInfoStr(id) + " 已淘汰");
        }
        else if(observedId != dealerId) {
            string output = playerInfoStr(observedId) + "的手牌資訊";
            output += " 點數為: " + to_string(players[observedId].getPoint());
            output += " "s + players[observedId].status.message;
            output += " 賭注為 ";
            if(players[observedId].status.betMultiple == -1) {
                output += to_string(players[id].bet * 2);
            }
            else {
                output += to_string(players[id].bet);
            }
            if(showEarned) {
                output += " 本回合";
                if(players[id].prevRoundGetMoney >= 0) {
                    output += " 獲得 ";
                }
                else {
                    output += " 失去 ";
                }
                output += to_string(abs(players[id].prevRoundGetMoney)) + " 元 ";
            }
            printer.insertLine(output);
            printer.insertPokerCards(players[observedId].cards);
        }
        printer.insertSeparateLine();
    }
    printer.print();
}

void Game::Round::setBet() {
    for (int id = 0; id < playerNum; id++ ) {
        if(id == dealerId) {
            if(game.isUser(id)) {
                string output;
                output += playerInfoStr(id);
                output += "無法下注";
                printer.insertLineInstant(output);
                pressEnterContinue();
            }
            continue;
        }
        else if(game.isUser(id) == false) {
            //ai
            if(players[id].eliminate()) {
                continue;
            }
            players[id].bet = max(MinimumBet, min((random(MinimumBet, players[id].getMoney() / 2) / 100) * 100, players[id].getMoney()));
        }
        else if(game.isUser(id)) {
            if(players[id].eliminate()) {
                continue;
            }
            string output;
            output += playerInfoStr(id);
            output += "請輸入要下注的金額  底注為 ";
            output += to_string(MinimumBet);
            output += "元 你目前有 ";
            output += to_string(players[id].getMoney());
            output += "元";
            printer.insertLine("");
            printer.insertLineInstant(output);
            int inputBet = getSafeNumInput(MinimumBet, players[id].getMoney());
            while(inputBet % BetElement != 0) {
                printer.insertLineInstant("賭注只能是 " + to_string(BetElement) + " 的倍數");
                inputBet = getSafeNumInput(MinimumBet, players[id].getMoney());
            }
            players[id].bet = inputBet;
            printer.insertLineInstant("#您將下注 " + to_string(players[id].bet) + "元  " + "將剩下 " + to_string(players[id].getMoney() - players[id].bet) + "元");
            pressEnterContinue();
        }
    }
}

string Game::Round::playerInfoStr(int id) {
    string res;
    res += "[玩家"s + to_string(id + 1);
    if(game.isUser(id)) {
        res += " #真人玩家";
        res += to_string(game.getUserSer(id));
        res += " # ";
    }
    else {
        res += " 電腦玩家 ";
    }
    if(id == dealerId) {
        res += " (莊家)";
    }
    res += "]\t";
    if(id != dealerId) {
        res += "\t";
    }
    return res;
}

string Game::Round::playerStatusInfoStr(int id) {
    Player &player = players[id];
    string res;
    res += playerInfoStr(id);
    res += player.status.message;
    if(player.status.betMultiple <= 0) {
        res += " 已獲得 " + to_string(player.getPoint()) + "點";
    }
    return res;
}