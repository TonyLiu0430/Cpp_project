#include "util.h"
#include <bits/stdc++.h>
using namespace std;

Exception::Exception(string message): message(message) {}

const char* Exception::what() const throw() {
    return message.c_str();
}

Point::Point(int x, int y): x(x), y(y) {}

Area::Area(int x, int y, int length, int width): x(x), y(y), length(length), width(width) {}

Area::Area(const pair<int, int> &coordinate, const pair<int, int> &size): 
 x(coordinate.first),
 y(coordinate.second),
 length(size.first),
 width(size.second) {}

bool Area::operator==(const Area &other) const {
    return x == other.x && y == other.y && length == other.length && width == other.width;
}

bool Area::has(int x, int y) {
    
}