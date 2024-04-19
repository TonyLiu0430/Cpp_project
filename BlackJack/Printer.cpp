#include "Printer.h"
#include "util.h"
using namespace std;

Printer printer;

void Printer::insertPokerCards(const std::vector<Poker> &cards) {
    if(screenBuf.size() < line + Poker::SIZE) {
        screenBuf.resize(line + Poker::SIZE, prefix + "\t");
    }
    for (const auto &card : cards) {
        vector<string> cardAppearence = card.appearance();
        for (int i = 0; i < Poker::SIZE; i++) {
            screenBuf[line + i] += cardAppearence[i] + "   ";
        }
    }
    line += Poker::SIZE;
    return;
}
void Printer::insertDealerPokerCards(const std::vector<Poker> &cards) {
    if(screenBuf.size() < line + Poker::SIZE) {
        screenBuf.resize(line + Poker::SIZE, prefix + "\t");
    }
    for (int i = 0; i < cards.size(); i++) {
        vector<string> cardAppearence;
        if(i == 1) {
            cardAppearence = Poker::backAppearance();
        }
        else {
            cardAppearence = cards[i].appearance();
        }
        for (int i = 0; i < Poker::SIZE; i++) {
            screenBuf[line + i] += cardAppearence[i] + "   ";
        }
    }
    line += Poker::SIZE;
    return;
}

void Printer::insertSeparateLine() {
    if(screenBuf.size() < line + 1) {
        screenBuf.resize(line + 1);
    }
    screenBuf[line] = "===================================================================================";
    screenBuf[line] += "===================================================================================";
    line += 1;
}

void Printer::insertLine(std::string content) {
    if(screenBuf.size() < line + 1) {
        screenBuf.resize(line + 1);
    }
    screenBuf[line] = prefix + content;
    line += 1;
}

void Printer::insertLineInstant(std::string content) {
    insertLine(content);
    print();
}

void Printer::print() {
    clearScreen();
    for(int i = 0; i < screenBuf.size(); i++ ) {
        cout << screenBuf[i] << endl;
    }
    cout << prefix;
}

void Printer::reset() {
    line = 0;
    index = 0;
    screenBuf = vector<string>();
}
