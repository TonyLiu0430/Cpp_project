#pragma once

/*interface*/
class UserInterface {
public:
    virtual void start() const = 0;
    virtual void showStart() = 0;
};