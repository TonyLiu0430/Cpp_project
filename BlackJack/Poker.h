#pragma once
#include <vector>
#include <string>
class Poker {
public:
    //std::vector<std::string> data;
    Poker(int num, int s);
    int num;
    int s;
    std::vector<std::string> appearance() const;
    static std::vector<std::string> backAppearance();
    static std::string number2Figure(int num);
    static const int RSIZE = 7;
    static const int SIZE = RSIZE + 3;
private:
    static const std::vector<std::vector<std::string>> pattern_img;
};