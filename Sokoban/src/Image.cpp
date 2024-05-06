#ifdef AWG
#include <bits/stdc++.h>
#include "Image.h"
#include "util.h"
using namespace std;

Image::Image(string name, int length, int width): name(name), length(length), width(width), path("resource\\" + name + ".bmp") {
    LPCSTR imagePath = path.c_str();
    hBitmap = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, length, width, LR_LOADFROMFILE);
}

ImageManager::ImageManager() {
    images.emplace("start_before", new Image("start_before"s, 300, 150));
    images.emplace("start_after", new Image("start_after"s, 300, 150));
    images.emplace("player", new Image("player"s, 30, 30));
    images.emplace("wall", new Image("wall"s, 30, 30));
    images.emplace("road", new Image("road"s, 30, 30));
    images.emplace("box", new Image("box"s, 30, 30));
    images.emplace("checkPoint", new Image("checkPoint"s, 30, 30));
    images.emplace("boxOnCheckPoint", new Image("boxOnCheckPoint"s, 30, 30));
}

ImageManager::~ImageManager() {
    for(auto &[name, image]: images) {
        delete image;
    }
}

Image* ImageManager::getImage(std::string name) {
    if(images.find(name) == images.end()) {
        throw Exception(name + " Image didn't initiallize OR not found");
    }
    return images[name];
}

ButtonLike::ButtonLike(std::string name, std::function<void(void)> action, ActionTag tag): name(name), action(action), tag(tag) {
    before = imageManager.getImage(name + "_before");
    after = imageManager.getImage(name + "_after");
    length = before->length;
    width = before->width;
}

#endif