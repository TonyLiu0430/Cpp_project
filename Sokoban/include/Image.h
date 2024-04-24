#pragma once
#ifdef AWG
#include "windowsBase.h"
#include <string>
#include <map>
#include <utility>
#include <memory>
#include "util.h"


class Image {
    inline static std::map<std::string, std::shared_ptr<Image>> allImage{};
public:
    Image(std::string name, int length, int width);
    static bool loadAllImage();
    const std::string path;
    const std::string name;
    const int length;
    const int width;
    static std::shared_ptr<Image> getImage(std::string name);
};

class ButtonLike {
public:
    int length;
    int width;
    std::shared_ptr<Image> before;
    std::shared_ptr<Image> after;
    std::function<void(void)> action;
    const std::string name;
    ButtonLike(std::string name, std::function<void(void)> action);
};



class ImageShower {
public:
    //std::vector<std::pair<std::shared_ptr<Image>, Point>> images;
    std::map<std::string, std::pair<std::shared_ptr<Image>, Point>> images;
    void show(HDC hdc);
    void clear();
    void insertImage(std::shared_ptr<Image> image, const Point &p);
    void changeImage(std::string name, std::shared_ptr<Image> image);
    void refreshInstant(HWND hWnd);
};

inline ImageShower imageShower;

#endif