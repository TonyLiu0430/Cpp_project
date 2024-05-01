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

template class Game<WindowUserInterface>;
template class Game<ConsoleUserInterface>;
