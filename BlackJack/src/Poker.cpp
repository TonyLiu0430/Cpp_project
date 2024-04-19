#include "Poker.h"
#include <bits/stdc++.h>

using namespace std;

Poker::Poker(int num, int s) : num(num), s(s){};
vector<string> Poker::appearance() const {
    string figure = number2Figure(num);
    vector<string> data(SIZE);
    data[0] = figure + " " + (figure.size() == 1 ? "-"s : ""s ) + "--------";// pattern_img[s][i]
    for(int i = 0; i < pattern_img[s].size(); i++ ) {
        data[i + 1] = (i == 0 ? " " : "|") + pattern_img[s][i] + (i == pattern_img[s].size() - 1 ? " " : "|");
    }
    data[RSIZE + 1] = "--------" + (figure.size() == 1 ? "-"s : ""s ) + " " + figure;
    return data;
}

vector<string> Poker::backAppearance() {
    string figure = "--";
    vector<string> data(SIZE);
    data[0] = figure + "---------";// pattern_img[s][i]
    for(int i = 0; i < pattern_img[4].size(); i++ ) {
        data[i + 1] = "|" + pattern_img[4][i] + "|";
    }
    data[RSIZE + 1] = "---------" + figure;
    return data;
}

string Poker::number2Figure(int num) {
    string figure;
    if(num == 1) {
        figure = "A";
    }
    else if(num <= 10) {
        figure = to_string(num);
    }
    else if(num == 11) {
        figure = "J";
        //figure = string() + (char)('J' + (num - 11)) + " ";
    }
    else if(num == 12) {
        figure = "Q";
    }
    else if(num == 13) {
        figure = "K";
    }
    return figure;
}

const vector<vector<string>> Poker::pattern_img = {
    {
        "    *    ",
        "   ***   ",
        " *  *  * ",
        "*********",
        " *  *  * ",
        "    *    ",
        "   ***   "
    },
    {
        "    *    ",
        "   ***   ",
        " ******* ",
        "*********",
        " ******* ",
        "   ***   ",
        "    *    "
    },
    {
        " *     * ",
        "***   ***",
        "*********",
        "*********",
        " ******* ",
        "  *****  ",
        "    *    "
    },
    {
        "    *    ",
        "   ***   ",
        " ******* ",
        "*********",
        "*** * ***",
        "    *    ",
        "   ***   "
    },
    {
        "*********",
        "*********",
        "*********",
        "*********",
        "*********",
        "*********",
        "*********"
    }
};
