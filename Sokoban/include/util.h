#pragma once
#include <exception>
#include <string>
#include <set>

class Exception: public std::exception {
    const std::string message;
public:
    virtual const char* what() const throw() override;
    Exception(std::string message);
};

struct Point {
    int x, y;
    Point(int x, int y);
    Point(): x(0), y(0) {}
    operator std::pair<int, int>() const {
        return std::make_pair(x, y);
    }
};


struct Area {
    int x, y, length, width;
    Area(int x, int y, int length, int width);
    Area(const std::pair<int, int> &coordinate, const std::pair<int, int> &size);
    bool isIn(int x, int y);
    bool isOut(int x, int y);
    bool operator==(const Area &other) const;
};



