#pragma once

#if defined(_WIN32)
    #ifdef EDITORCORE_EXPORTS
        #define EDITORCORE_API __declspec(dllexport)
    #else
        #define EDITORCORE_API __declspec(dllimport)
    #endif
#elif defined(__ANDROID__)
    #ifdef EDITORCORE_EXPORTS
        #define EDITORCORE_API __attribute__((visibility("default")))
    #else
        #define EDITORCORE_API
    #endif
#else
    #define EDITORCORE_API
#endif

//Entry Points
#if defined(__ANDROID__)
    #define START_GAME() \
        __attribute__((constructor)) static void on_library_loaded()
#else
    #define START_GAME() \
        int main()
#endif

extern "C" EDITORCORE_API int RunEditor();
extern "C" EDITORCORE_API int RunGame();