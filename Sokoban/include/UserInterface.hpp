#define USE_UI_BASE 0
#if USE_UI_BASE
#pragma once
#include <vector>

template<class T>
class UserInterface {
public:
    using UI = T;
    UserInterface() {
        static_cast<T*>(this)->init();
    }
    void start() {
        static_cast<T*>(this)->start();
    }
    void showStart() {
        static_cast<T*>(this)->showStart();
    }
    void showBoard(const std::vector<std::vector<char>> &board) {
        static_cast<T*>(this)->showBoard(board);
    }
    void end() {
        static_cast<T*>(this)->end();
    }
    void startMessageLoop() {
        static_cast<T*>(this)->startMessageLoop();
    }
    void stopMessageLoop() {
        static_cast<T*>(this)->stopMessageLoop();
    }
};
#endif