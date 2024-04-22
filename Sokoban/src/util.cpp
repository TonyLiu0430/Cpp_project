#include "util.h"
#include <bits/stdc++.h>
using namespace std;

Exception::Exception(string message): message(message) {}

const char* Exception::what() const throw() {
    return message.c_str();
}