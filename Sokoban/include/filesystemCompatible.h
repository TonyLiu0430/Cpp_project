#pragma once

#include <vector>
#include <string>

#ifndef FILESYSTEM_SUPPORTED
#if defined(__has_include) && __has_include(<filesystem>)
#define FILESYSTEM_SUPPORTED 1
#endif
#endif

#ifdef FILESYSTEM_SUPPORTED
#include <filesystem>

namespace fs = std::filesystem;
#else

#ifdef _WIN32
#include <windows.h>
#endif // ifdef _WIN32
#warning STD::FILESYSTEM NOT SUPPORTED USING WINDOWS API TO GET FILE LIST
namespace fs {
    class path: public std::string {
    public:
        template<class ...Args>
        path(Args&& ...args): std::string(std::forward<Args>(args)...) {}
        path filename() const {
            return substr(find_last_of('\\') + 1);
        }
        path string() const {
            return *this;
        }
    };
}

#ifndef _WIN32
#error LINUX OR MACOS NOT SUPPORTED PLEASE USE NEWER COMPILER BY SUPPORTED C++17 STD::FILESYSTEM
#endif

#endif // if defined(__has_include) && __has_include(<filesystem>)

std::vector<fs::path> getBoardList(std::string dirPath);

