#pragma once
#ifdef AWG
#ifdef UNICODE
#undef UNICODE
#endif
#ifndef ASCII
#define ASCII
#endif

#include <windows.h>
#include <string>
#include <map>
#include <utility>
#include <memory>
#include <functional>
#include "util.h"


class Image {
    Image(std::string name, int length, int width);
    friend class ImageManager;
public:
    HBITMAP hBitmap;
    const std::string path;
    const std::string name;
    const int length;
    const int width;
};

class ImageManager {
    std::map<std::string, Image*> images;
public:
    Image* getImage(std::string name);
    ImageManager();
    ~ImageManager();
};

inline ImageManager imageManager;

class ButtonLike {
public:
    int length;
    int width;
    Image* before;
    Image* after;
    std::function<void(void)> action;
    const std::string name;
    enum class ActionTag {
        once,
        repeat
    };
    ActionTag tag;
    ButtonLike(std::string name, std::function<void(void)> action, ActionTag tag = ActionTag::once);
};


#endif