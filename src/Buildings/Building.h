#ifndef SDL_BASEGAME_BUILDING_H
#define SDL_BASEGAME_BUILDING_H


#include "Units/Unit.h"

class Building {
public:
    // ctor
private:
    Unit *_capturing;
    int _captureProgess = 20;
    int _team;
    int _tileIndex;

};


#endif //SDL_BASEGAME_BUILDING_H
