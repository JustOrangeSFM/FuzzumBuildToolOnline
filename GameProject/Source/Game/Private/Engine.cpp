//GlobalEngineIncludes
#include "EngineCore.h"


//MainGame
START_GAME()
{
#ifndef __ANDROID__
    return 0;
#endif
}