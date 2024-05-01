#pragma once

template<class T>
class UserInterface {
public:
    UserInterface() {
        static_cast<T*>(this)->init();
    }
    void start() {
        static_cast<T*>(this)->start();
    }
    void showStart() {
        static_cast<T*>(this)->showStart();
    }
};