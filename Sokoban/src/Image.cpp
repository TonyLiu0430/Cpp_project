#ifdef AWG
#include "Image.h"
#include "util.h"
#include <bits/stdc++.h>
using namespace std;

Image::Image(string name, int length, int width): name(name), length(length), width(width), path("resource\\" + name + ".bmp") {}

static bool ___________init_temp_______________ = Image::loadAllImage();

bool Image::loadAllImage() {
    allImage.emplace("man", make_shared<Image>("resource\\man.bmp", 70, 70));
    return true;
}

shared_ptr<Image> Image::getImage(std::string name) {
    if(allImage.find(name) == allImage.end()) {
        throw Exception(name + " Image didn't initiallize OR not found");
    }
    return allImage[name];
}



void ImageShower::show(HDC hdc) {
    for(auto &[image, point]: images) {
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

void ImageShower::insertImage(shared_ptr<Image> image, const Point &p) {
    images.push_back({image, p});
}

void ImageShower::refreshInstant(HWND hWnd) {
    RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

ButtonLike::ButtonLike(std::string name, std::function<void(void)> action): name(name), action(action) {
    before = Image::getImage(name + "_before");
    after = Image::getImage(name + "_after");
    length = before->length;
    width = before->width;
}

#endif