#include "util.h"
#include <bits/stdc++.h>
#include "filesystemCompatible.h"
using namespace std;

Exception::Exception(string message): message(message) {}

const char* Exception::what() const throw() {
    return message.c_str();
}

Point::Point(int x, int y): x(x), y(y) {}

Area::Area(int x, int y, int length, int width): x(x), y(y), length(length), width(width) {}

Area::Area(const pair<int, int> &coordinate, const pair<int, int> &size): 
 x(coordinate.first),
 y(coordinate.second),
 length(size.first),
 width(size.second) {}

bool Area::operator==(const Area &other) const {
    return x == other.x && y == other.y && length == other.length && width == other.width;
}

bool Area::isIn(int px, int py) {
    return x <= px && px <= x + length && y <= py && py <= y + width;
}

bool Area::isOut(int px, int py) {
    return !isIn(px, py);
}

#ifdef FILESYSTEM_SUPPORTED
std::vector<fs::path> getBoardList_std_filesystem(std::string dirPath) {
    if(!fs::exists(dirPath)) {
        throw Exception("missions folder not found");
    }
    fs::directory_entry dir(dirPath);
    if(!dir.is_directory()) {
        throw Exception("missions is not a directory");
    }
    std::vector<fs::path> boardList;
    fs::directory_iterator dirIt(dirPath);
    for(auto &p: dirIt) {
        if(p.is_regular_file()) {
            boardList.push_back(p);
        }
    }
    std::sort(boardList.begin(), boardList.end(), [](const fs::path &a, const fs::path &b) {
        std::string as = a.filename().string();
        std::string bs = b.filename().string();
        return as.size() == bs.size() ? as < bs : as.size() < bs.size();
    });
    return boardList;
}
#else

std::vector<fs::path> getBoardList_win32(std::string dirPath) {
    clog << "無法使用std::filesystem標準庫, 使用windows api獲取檔案列表\n" << "有可能會有非預期的錯誤\n";
    std::vector<fs::path> result;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((dirPath + "/*").c_str(), &findFileData);
    if(hFind == INVALID_HANDLE_VALUE) {
        throw Exception("missions folder not found");
    }
    do {
        if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
        result.push_back(findFileData.cFileName);
    } while(FindNextFile(hFind, &findFileData));
    FindClose(hFind);
    std::sort(result.begin(), result.end(), [](const fs::path &a, const fs::path &b) {
        return a.size() == b.size() ? a < b : a.size() < b.size();
    });

    for(auto &p: result) {
        p = dirPath + "\\" + p;
    }

    return result;
}
#endif

std::vector<fs::path> getBoardList(std::string dirPath) {
    #ifdef FILESYSTEM_SUPPORTED
    return getBoardList_std_filesystem(dirPath);
    #else
    return getBoardList_win32(dirPath);
    #endif
}