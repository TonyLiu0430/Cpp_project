#pragma once
#include <map>
#include <string>

class AiStrategy {
    //ref : https://www.blackjackinfo.com/blackjack-basic-strategy-engine/?numdecks=1&soft17=h17&dbl=all&das=no&surr=es&peek=no
    std::map<int, std::map<int, std::string>> hard;
    std::map<int, std::map<int, std::string>> soft;
    void genHard();
    void genSoft();
public:
    AiStrategy();
    std::string hardStrategy(int dealerCard, int playerPoint);
    std::string softStrategy(int dealerCard, int playerPoint);
};

extern AiStrategy aiStrategy;

