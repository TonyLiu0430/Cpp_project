#pragma once
#include "Poker.h"
#include <vector>

class PokerCardDeck {
    std::vector<Poker> deck;

public:
    PokerCardDeck();
    Poker distributeCard();
};