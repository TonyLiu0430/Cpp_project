#ifdef AWG
#include <bits/stdc++.h>
#include "Image.h"
#include "util.h"
#include "windowsBase.h"
using namespace std;

Image::Image(string name, int length, int width): name(name), length(length), width(width), path("resource\\" + name + ".bmp") {
    LPCSTR imagePath = path.c_str();
    imageHandle = LoadImage(NULL, imagePath, IMAGE_BITMAP, length, width, LR_LOADFROMFILE);
    if(imageHandle == NULL) {
        throw UnexpectedException(name + " image not Found or load failed");
    }
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
    images.emplace("playerOnCheckPoint", new Image("playerOnCheckPoint"s, Image::GameObjLen, Image::GameObjLen));
    /*Game Obj Image*/

    /*Game status Image*/
    images.emplace("win", new Image("win"s, 500, 300));
    images.emplace("lose", new Image("lose"s, 500, 300));
    images.emplace("pressEnter", new Image("pressEnter"s, 500, 300));
    /*Game status Image*/

    /*Board Choose Button Image*/
    for (int i = 1; i <= 5; i++ ) {
        string imageName = "mission" + to_string(i);
        images.emplace(imageName + "_before", new Image(imageName + "_before", 80, 80));
        images.emplace(imageName + "_after", new Image(imageName + "_after", 80, 80));
    }
    images.emplace("default_before", new Image("default_before"s, 80, 80));
    images.emplace("default_after", new Image("default_after"s, 80, 80));
    /*Board Choose Button Image*/
    images.emplace("chooseLevel", new Image("chooseLevel"s, Image::statusLen, Image::statusWidth));
    images.emplace("playing", new Image("playing"s, Image::statusLen, Image::statusWidth));
    /*Board Choose Button Image*/

    /*Button*/
    images.emplace("surrender_before", new Image("surrender_before"s, 150, 150));
    images.emplace("surrender_after", new Image("surrender_after"s, 150, 150));
    //
    images.emplace("return_before", new Image("return_before"s, 150, 100));
    images.emplace("return_after", new Image("return_after"s, 150, 100));
    /*Button*/
}

ImageManager::~ImageManager() {
    for(auto &[name, image]: images) {
        delete image;
    }
}

Image* ImageManager::getImage(std::string name) {
    if(images.find(name) == images.end()) {
        throw UnexpectedException(name + " Image didn't initiallize OR not found");
    }
    return images[name];
}

bool ImageManager::hasImage(std::string name) {
    return images.find(name) != images.end();
}

ButtonLike::ButtonLike(std::string name, std::string imageName, std::function<void(void)> action, ActionTag tag, ButtonStyle style) 
    : name(name)
    , imageName(imageName)
    , action(action)
    , tag(tag)
    , style(style) {
    before = imageManager.getImage(imageName + "_before");
    after = imageManager.getImage(imageName + "_after");
    length = before->length;
    width = before->width;
}

ButtonLike::ButtonLike(std::string name, std::function<void(void)> action, ActionTag tag): ButtonLike(name, name, action, tag) {}

void ButtonLike::insertToWindow(Window *window, ButtonLike button, Point p) {
    Area area({p.x, p.y}, {button.length, button.width});
    buttonAreas.insert({button.name, area});
    if(button.style == ButtonStyle::withText) {
        window->textShower.insertText(button.name, area);
    }
    window->mouseProcesser.moveIn.insertEvent(area, [=]() {
        if(window->imageShower.removeImage(button.name + "_before")) {
            window->imageShower.insertImage(button.name + "_after", button.after, {p.x, p.y});
            window->imageShower.refreshArea(area);
        }
    });
    window->mouseProcesser.moveOut.insertEvent(area, [=]() {
        if(window->imageShower.removeImage(button.name + "_after")) {
            window->imageShower.insertImage(button.name + "_before", button.before, {p.x, p.y});
            window->imageShower.refreshArea(area);
        }
    });
    window->mouseProcesser.click.insertEvent(area, [=]() {
        if(button.tag == ButtonLike::ActionTag::once) {
            deleteFromWindow(window, button.name);
        }
        button.action();
    });
    //cerr << "INSERT " << button.name << " <-> " << button.before->name << endl;
    window->imageShower.insertImage(button.name + "_before", button.before, {p.x, p.y});
    window->imageShower.refreshArea(area);
}

void ButtonLike::deleteFromWindow(Window *window, string name) {
    if(buttonAreas.find(name) == buttonAreas.end()) {
        return;
    }
    Area area = buttonAreas[name];
    window->mouseProcesser.moveOut.removeEvent(area);
    window->mouseProcesser.moveIn.removeEvent(area);
    window->mouseProcesser.click.removeEvent(area);

    /***/window->textShower.removeText(name);
    if(window->imageShower.removeImage(name + "_before") | window->imageShower.removeImage(name + "_after")) {
        window->imageShower.refreshArea(area);
    }
    buttonAreas.erase(name);
}

void ButtonLike::deleteAllFromWindow(Window *window) {
    for(auto &[name, area]: buttonAreas) {
        window->mouseProcesser.moveOut.removeEvent(area);
        window->mouseProcesser.moveIn.removeEvent(area);
        window->mouseProcesser.click.removeEvent(area);

        /***/window->textShower.removeText(name);
        if(window->imageShower.removeImage(name + "_before") | window->imageShower.removeImage(name + "_after")) {
            window->imageShower.refreshArea(area);
        }
    }
    buttonAreas.clear();
}



#endif