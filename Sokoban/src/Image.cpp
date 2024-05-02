#ifdef AWG
#include <bits/stdc++.h>
#include "Image.h"
#include "util.h"
using namespace std;

Image::Image(string name, int length, int width): name(name), length(length), width(width), path("resource\\" + name + ".bmp") {}

ImageManager::ImageManager() {
    images.emplace("man", new Image("man"s, 70, 70));
    images.emplace("start_before", new Image("start_before"s, 300, 150));
    images.emplace("start_after", new Image("start_after"s, 300, 150));
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