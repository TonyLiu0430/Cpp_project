#ifdef AWG
#include "Image.h"
#include "util.h"
#include <bits/stdc++.h>
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


void ImageShower::show(HDC hdc) {
    for(auto &[name, imgA]: images) {
        auto &[image, point] = imgA;
        HDC mdc = CreateCompatibleDC(hdc);
        LPCSTR imagePath = image->path.c_str();
        HBITMAP bg = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, image->length, image->width, LR_LOADFROMFILE);
            
        SelectObject(mdc,bg);
        BitBlt(hdc, point.x, point.y, image->length, image->width, mdc, 0, 0, SRCAND);
    }
}

void ImageShower::clear() {
    images.clear();
}

int ImageShower::insertImage(Image* image, const Point &p) {
    images[image->name] = {image, p};
    return 1;
}

int ImageShower::removeImage(std::string name) {
    if(images.find(name) != images.end()) {
        cout << "remove " << name << endl;
        images.erase(name);
        return 1;
    }
    return 0;
}


void ImageShower::refreshInstant(HWND hWnd) {
    RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

void ImageShower::refreshArea(HWND hWnd, const Area &area) {
    RECT rect = {(LONG)area.x, (LONG)area.y, (LONG)area.x + area.length, (LONG)area.y + area.width};
    cout << "refresh " << area.x << " " << area.y << " " << area.length << " " << area.width << endl;
    InvalidateRect(hWnd, &rect, true);
    UpdateWindow(hWnd);
}

ButtonLike::ButtonLike(std::string name, std::function<void(void)> action, ActionTag tag): name(name), action(action), tag(tag) {
    before = imageManager.getImage(name + "_before");
    after = imageManager.getImage(name + "_after");
    length = before->length;
    width = before->width;
}

#endif