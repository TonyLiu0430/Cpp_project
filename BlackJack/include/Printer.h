#ifndef PRINTER_H_INCLUDED
#define PRINTER_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Poker.h"
using namespace std;

class Poker;
class Printer {
public:
    //void insertPokerCard(const Poker&);
    void insertPokerCards(const std::vector<Poker>&);
    void insertDealerPokerCards(const std::vector<Poker>&);
    void insertLine(std::string);
    void insertLineInstant(std::string content);
    void print();
    void reset();
    void insertSeparateLine();
private:
    const std::string prefix = "\t\t\t\t";

    int line = 0;
    int index = 0;
    vector<string> screenBuf;
};

extern Printer printer;
#endif // PRINTER_H_INCLUDED
