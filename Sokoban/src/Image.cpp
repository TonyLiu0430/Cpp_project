#ifdef AWG
#include <bits/stdc++.h>
#include "Image.h"
#include "util.h"
using namespace std;

Image::Image(string name, int length, int width): name(name), length(length), width(width), path("resource\\" + name + ".bmp") {
    LPCSTR imagePath = path.c_str();
    imageHandle = LoadImage(NULL, imagePath, IMAGE_BITMAP, length, width, LR_LOADFROMFILE);
}

Image::~Image() {
    for(auto &image: imagePool) {
        DeleteObject(image);
    }
    DeleteObject(imageHandle);
}

HBITMAP Image::getHBitmap() {
    if(instanceCnt < imagePool.size()) {
        return imagePool[instanceCnt++];
    }
    HBITMAP res = (HBITMAP)CopyImage(
        imageHandle,
        IMAGE_BITMAP,
        0,
        0,
        0
    );
    if(res == NULL) {
        cerr << name << " image copy failed\n";
    }
    imagePool.push_back(res);
    instanceCnt++;
    return res;
}

void Image::resetInstances() {
    instanceCnt = 0;
}

ImageManager::ImageManager() {
    /*start button image*/
    images.emplace("start_before", new Image("start_before"s, 300, 150));
    images.emplace("start_after", new Image("start_after"s, 300, 150));
    /*start button image*/

    /*Game Obj Image*/
    images.emplace("player", new Image("player"s, Image::GameObjLen, Image::GameObjLen));
    images.emplace("wall", new Image("wall"s, Image::GameObjLen, Image::GameObjLen));
    images.emplace("road", new Image("road"s, Image::GameObjLen, Image::GameObjLen));
    images.emplace("box", new Image("box"s, Image::GameObjLen, Image::GameObjLen));
    images.emplace("checkPoint", new Image("checkPoint"s, Image::GameObjLen, Image::GameObjLen));
    images.emplace("boxOnCheckPoint", new Image("boxOnCheckPoint"s, Image::GameObjLen, Image::GameObjLen));
    /*Game Obj Image*/

    /*Game status Image*/
    images.emplace("win", new Image("win"s, 500, 300));
    /*Game status Image*/

    /*Board Choose Button Image*/
    images.emplace("mission1_before", new Image("mission1_before"s, 70, 70));
    images.emplace("mission1_after", new Image("mission1_after"s, 70, 70));
    images.emplace("mission2_before", new Image("mission2_before"s, 70, 70));
    images.emplace("mission2_after", new Image("mission2_after"s, 70, 70));
    images.emplace("mission3_before", new Image("mission3_before"s, 70, 70));
    images.emplace("mission3_after", new Image("mission3_after"s, 70, 70));
    images.emplace("mission4_before", new Image("mission4_before"s, 70, 70));
    images.emplace("mission4_after", new Image("mission4_after"s, 70, 70));
    /*default*/
    //images.emplace("default_mission_before", new Image("default_before"s, 70, 70));
    /*Board Choose Button Image*/
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

bool ImageManager::hasImage(std::string name) {
    return images.find(name) != images.end();
}

ButtonLike::ButtonLike(std::string name, std::function<void(void)> action, ActionTag tag): name(name), action(action), tag(tag) {
    before = imageManager.getImage(name + "_before");
    after = imageManager.getImage(name + "_after");
    length = before->length;
    width = before->width;
}

#endif