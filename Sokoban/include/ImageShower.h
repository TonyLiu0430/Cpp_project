#pragma once
#ifdef _WIN32
#include "windowsBase.h"
#include <string>
#include <map>
#include <utility>
#include <memory>

class Image {
    inline static std::map<std::string, std::shared_ptr<Image>> allImage{};
public:
    Image(std::string path, int length, int width);
    static bool loadAllImage();
    const std::string path;
    const int length;
    const int width;
    static std::shared_ptr<Image> getImage(std::string name);
};

class Point {
public:
    int x, y;
    Point(int x, int y);
};

class ImageShower {
public:
    std::vector<std::pair<std::shared_ptr<Image>, Point>> images;
    void show(HDC hdc);
    void clear();
    void insertImage(std::shared_ptr<Image> image, const Point &p);
    void refreshInstant(HWND hWnd);
};

inline ImageShower imageShower;

#endif