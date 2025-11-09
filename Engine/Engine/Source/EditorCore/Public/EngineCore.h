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

extern "C" EDITORCORE_API int RunEditor();
extern "C" EDITORCORE_API int RunGame();