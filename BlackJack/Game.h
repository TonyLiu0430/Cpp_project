#pragma once
#include <vector>
#include <map>
#include "Player.h"
#include "PokerCardDeck.h"

#define MinimumBet 10
#define BetElement 10


//extern int PLAYER_ID;

class Game {
    class Round {
        Game &game;
        PokerCardDeck deck;
        const int dealerId;
        const int playerNum;
        std::vector<Player> &players;
        void init();
        void showInfo(bool showDealer = false, bool showEarned = false);
        void setBet();
        void distributeCard(Player &player);
        void play(Player &player, int id);
        void playDealer(Player &player);
        void aiPlay(Player &player, int id);
        void aiDealer(Player &player);
        void hit(Player &player);
        void surrender(Player &player);
        void doubleDown(Player &player);
        void calculateEarned();
        void showRoundEndInfo();
        void insure(Player &player, int id);
        std::vector<int> insuranceIds;
        std::string playerInfoStr(int id);
        std::string playerStatusInfoStr(int id);
    public:
        Round(Game &game, int dealerId, std::vector<Player> &players);
        void run();
    };
    std::map<int, int> users;
    const int playerNum;
    std::vector<Player> players;
    const int roundNum;
    int getLargestPrevRoundMoneyPlayerId();
public:
    Game(int roundNum, int playerNum);
    int start();
    void showGameEndInfo();

    std::map<int, int> &getUsers();
    int getUserSer(int id);
    bool isUser(int id);
};
