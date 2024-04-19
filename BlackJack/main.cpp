#include <bits/stdc++.h>
#include "Game.h"
#include "util.h"
#include "Printer.h"
using namespace std;


int main() {
    while(true){
        srand(time(NULL));
        printer.insertLineInstant("請輸入局數");
        int roundNum = getSafeNumInput(1, 10);
        printer.insertLineInstant("#遊戲局數為:" + to_string(roundNum));
        printer.insertLine("");

        printer.insertLineInstant("請輸入總玩家人數 (真人 + AI) ");
        int playerNum = getSafeNumInput(2, 10);
        printer.insertLineInstant("#總玩家人數為:" + to_string(playerNum));
        printer.insertLine("");
        Game game(roundNum, playerNum);

        printer.insertLineInstant("請輸入真人玩家人數");
        //PLAYER_ID = getSafeNumInput(1, playerNum) - 1;
        auto &users = game.getUsers();
        int userNum = getSafeNumInput(1, playerNum);
        printer.insertLineInstant("#真人玩家人數為:" + to_string(userNum));
        printer.insertLine("");

        for (int i = 0; i < userNum; i++ ) {
            printer.insertLineInstant("請輸入真人玩家" + to_string(i + 1) + " 要成為幾號玩家");
            int id = getSafeNumInput(1, playerNum);
            while(game.isUser(id - 1)) {
                printer.insertLineInstant("重複輸入 請再次輸入");
                id = getSafeNumInput(1, playerNum);
            }
            users[id - 1] = i + 1;
            printer.insertLineInstant("#真人玩家" + to_string(i + 1) + " 將成為 " + to_string(id) + "號玩家");
            printer.insertLine("");
        }
        printer.insertLine("");

        pressEnterContinue();
        game.start();
    }
}

