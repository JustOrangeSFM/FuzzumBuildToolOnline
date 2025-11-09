//GlobalEngineIncludes
#include "EngineCore.h"


//Entry Points
#ifdef __ANDROID__
	
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