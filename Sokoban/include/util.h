#pragma once
#include <exception>
#include <string>

class Exception: public std::exception {
    const std::string message;
public:
    virtual const char* what() const throw() override;
    Exception(std::string message);
};

struct Point {
    int x, y;
    Point(int x, int y);
};


struct Area {
    int x, y, length, width;
    Area(int x, int y, int length, int width);
    Area(const pair<int, int> &coordinate, const pair<int, int> &size);
    bool has(int x, int y);
    bool operator==(const Area &other) const;
};

template<class T>
bool inSet(const T &a, std::set<T> s) {
    return s.find(a) != s.end();
}

template<class T, class U>
bool inSet(const T &a, const U &s) {
    return s.find(a) != s.end();
}