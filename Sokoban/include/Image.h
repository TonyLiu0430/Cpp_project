#pragma once
#ifdef AWG
#include "windowsBase.h"
#include <string>
#include <map>
#include <utility>
#include <memory>
#include "util.h"


class Image {
    //inline static std::map<std::string, std::shared_ptr<Image>> allImage{};
    //inline static std::map<std::string, Image*> allImage{};
    Image(std::string name, int length, int width);
    friend class ImageManager;
public:
    //static bool loadAllImage();
    const std::string path;
    const std::string name;
    const int length;
    const int width;
    //static std::shared_ptr<Image> getImage(std::string name);
    //static Image* getImage(std::string name);
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
    /*
    std::shared_ptr<Image> before;
    std::shared_ptr<Image> after;*/
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



class ImageShower {
public:
    //std::vector<std::pair<std::shared_ptr<Image>, Point>> images;
    std::map<std::string, std::pair<Image*, Point>> images;
    void show(HDC hdc);
    void clear();
    int insertImage(Image* image, const Point &p);
    int removeImage(std::string name);
    void refreshArea(HWND hWnd, const Area &area);
    void refreshInstant(HWND hWnd);
};

inline ImageShower imageShower;

#endif