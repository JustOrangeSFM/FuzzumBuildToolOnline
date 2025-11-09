#pragma once
#include <string>
#include <memory>

#if defined(_WIN32)
#include <windows.h>
using LibHandle = HMODULE;
#elif defined(__ANDROID__) || defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
using LibHandle = void*;
#else
#error "Unsupported platform"
#endif

class DynamicLibrary
{
public:
    explicit DynamicLibrary(const std::string& path);
    ~DynamicLibrary();

    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;

    template<typename T>
    T GetFunction(const char* name) const
    {
#if defined(_WIN32)
        return reinterpret_cast<T>(GetProcAddress(handle_, name));
#elif defined(__ANDROID__) || defined(__linux__) || defined(__APPLE__)
        return reinterpret_cast<T>(dlsym(handle_, name));
#endif
    }

    bool IsValid() const { return handle_ != nullptr; }

private:
    LibHandle handle_ = nullptr;
};