//Includes
#ifdef __ANDROID__
    #include <android/log.h>
    #include <pthread.h>
    #include <thread>        
    #include <chrono>        
#else
    #include <iostream>
    #include <thread>
    #include <chrono>
#endif


//GlobalEngineIncludes
#include "EngineCore.h"


//Entry Points
#ifdef __ANDROID__

    #define LOG_TAG "Engine"
    #define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)



   //долга работать temp trash
  /*  static void* background_work(void* arg) {
		LOG_INFO("Background thread started");
		for (int i = 0; i < 5; ++i) {
			LOG_INFO("Working... step %d", i + 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		LOG_INFO("Background work finished");
		return nullptr;
	}
*/
    
	
	// Autorun android 
    __attribute__((constructor))
    static void on_library_loaded() {
        
    }

#else

    // entry point pc
    int main() {
        return RunEditor();
    }

#endif