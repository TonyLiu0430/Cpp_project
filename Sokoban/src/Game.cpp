#include "Game.h"
#include "UserInterface.hpp"
#include "ConsoleUserInterface.h"
#include "WindowUserInterface.h"
#include <bits/stdc++.h>
using std::cout;
using namespace std;

template<class T>
void Game<T>::start() {
    ui.showStart();
    ui.start();

    std::cout << "end" << std::endl;
}

template<class T>
void Game<T>::loadBoard(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        throw Exception("Map file not found");
    }
    
    
}

template class Game<WindowUserInterface>;
template class Game<ConsoleUserInterface>;
