#include "Printer.h"
using namespace std;

Printer printer;

void Printer::insertSeparateLine() {
    if(screenBuf.size() < line + 1) {
        screenBuf.resize(line + 1);
    }
    screenBuf[line] = "===================================================================================";
    screenBuf[line] += "===================================================================================";
    line += 1;
}

void Printer::insertLine(std::string content) {
    if(screenBuf.size() < line + 1) {
        screenBuf.resize(line + 1);
    }
    screenBuf[line] = prefix + content;
    line += 1;
}

void Printer::insertLineInstant(std::string content) {
    insertLine(content);
    print();
}

void Printer::print() {
    system("cls");
    for(int i = 0; i < screenBuf.size(); i++ ) {
        cout << screenBuf[i] << endl;
    }
    cout << prefix;
}

void Printer::reset() {
    line = 0;
    index = 0;
    screenBuf = vector<string>();
}
