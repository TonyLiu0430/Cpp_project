#pragma once
#ifdef AWG

#include "windowsInclude.h"
#include <string>
#include <map>
#include <utility>
#include <memory>
#include <functional>
#include "util.h"


class Image {
    Image(std::string name, int length, int width);
    ~Image();
    friend class ImageManager;
    HANDLE imageHandle;
    int instanceCnt = 0;
    std::vector<HBITMAP> imagePool;
public:
    inline static const int GameObjLen = 40;
    inline static const int statusLen = 300;
    inline static const int statusWidth = 150;
    void resetInstances();
    HBITMAP getHBitmap();
    const std::string path;
    const std::string name;
    const int length;
    const int width;
};

class ImageManager {
    std::map<std::string, Image*> images;
public:
    Image* getImage(std::string name);
    bool hasImage(std::string name);
    ImageManager();
    ~ImageManager();
};

inline ImageManager imageManager;


class Window;
class ButtonLike {
    inline static std::map<std::string, Area> buttonAreas{};
public:
    int length;
    int width;
    Image* before;
    Image* after;
    std::function<void(void)> action;
    const std::string name;
    const std::string imageName;
    enum class ActionTag {
        once,
        repeat
    };
    enum class ButtonStyle {
        onlyImage,
        withText
    };
    ActionTag tag;
    ButtonStyle style;
    ButtonLike(std::string imageName, std::function<void(void)> action, ActionTag tag = ActionTag::once);
    ButtonLike(std::string name, std::string imageName, std::function<void(void)> action, ActionTag tag = ActionTag::once, ButtonStyle style = ButtonStyle::onlyImage);

    static void insertToWindow(Window *window, ButtonLike button, Point p);
    static void deleteFromWindow(Window *window, std::string name);
    static void deleteAllFromWindow(Window *window);
};


#endif