#pragma once

#include <vector>
#include <string>

#if defined(__has_include) && __has_include(<filesystem>)
#define FILESYSTEM_SUPPORTED 1
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
            return this->substr(this->find_last_of('\\') + 1);
        }
        path string() const {
            return *this;
        }
    };
}

#ifndef _WIN32
#error LINUX_OR_MACOS_NOT_SUPPORTED_PLEASE_USE_NEWER_COMPILER_BY_SUPPORTED_C++17_FILESYSTEM
#endif

#endif // if defined(__has_include) && __has_include(<filesystem>)

std::vector<fs::path> getBoardList(std::string dirPath);

