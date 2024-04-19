#include "PokerCardDeck.h"
#include "util.h"
#include <bits/stdc++.h>
using namespace std;

PokerCardDeck::PokerCardDeck() {
    for(int i = 1; i <= 13; i++ ) {
        for(int j = 0; j < 4; j++ ) {
            deck.push_back(Poker(i, j));
        }
    }
}

Poker PokerCardDeck::distributeCard() {
    assert(deck.size() != 0);
    int ord = random(deck.size());
    Poker res = deck[ord];
    deck.erase(deck.begin() + ord);
    return res;
}