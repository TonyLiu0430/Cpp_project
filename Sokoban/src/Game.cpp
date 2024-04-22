#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

Game::Game(std::unique_ptr<UserInterface> &ui): ui(move(ui)) {

}

void Game::start() {
    ui->showStart();
    ui->start();
    cout << "end" << endl;
}