#include "AiStrategy.h"
#include "Printer.h"
#include <bits/stdc++.h>
using namespace std;

AiStrategy::AiStrategy() {
    genHard();
    genSoft();
}

string AiStrategy::hardStrategy(int dealerCard, int playerPoint) {
    if(dealerCard > 10) {
        dealerCard = 10;
    }
    return hard[dealerCard][playerPoint];
}

string AiStrategy::softStrategy(int dealerCard, int playerPoint) {
    dealerCard = min(10, dealerCard);
    playerPoint = min(10, playerPoint);
    return soft[dealerCard][playerPoint];
}

void AiStrategy::genHard() {
    ifstream in;
    #ifdef _WIN32
    in.open("strategy\\hardStrategy.txt");
    #else /*Linux*/
    in.open("strategy/hardStrategy.txt");
    #endif
    if(in.fail()) {
        printer.insertLineInstant("Ai策略設定檔缺失");
        assert(false);
    }
    vector<int> dealer(10);
    for (int i = 0; i < 10; i++ ) {
        in >> dealer[i];
    }
    for (int i = 0; i < 18; i++) {
        int handCard;
        in >> handCard;
        for (int j = 0; j < 10; j++ ) {
            string s;
            in >> s;
            hard[dealer[j]][handCard] = s;
        }
    }
    in.close();
}

void AiStrategy::genSoft() {
    ifstream in;
    #ifdef _WIN32
    in.open("strategy\\softStrategy.txt");
    #else /*Linux*/
    in.open("strategy/softStrategy.txt");
    #endif
    if(in.fail()) {
        printer.insertLineInstant("Ai策略設定檔缺失");
        assert(false);
    }
    vector<int> dealer(10);
    for (int i = 0; i < 10; i++ ) {
        in >> dealer[i];
    }
    for (int i = 0; i < 10; i++) {
        int handCard;
        in >> handCard;
        for (int j = 0; j < 10; j++ ) {
            string s;
            in >> s;
            soft[dealer[j]][handCard] = s;
        }
    }
    in.close();
}

AiStrategy aiStrategy;