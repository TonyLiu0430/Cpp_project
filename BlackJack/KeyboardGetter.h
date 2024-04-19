#pragma once
#include <map>
#include <functional>
#include "Printer.h"

class KeyboardGetter {
    std::map<char, std::function<void(void)>> charCallback;
    std::vector<std::string> charPrompt;
    bool called = false;
public:
    void insertCharEvent(char ch, std::string prompt, std::function<void(void)> callback);
    void insertCharEvent(char ch, std::vector<std::string> prompts, std::function<void(void)> callback);
    void get();
    ~KeyboardGetter();
};