#include "../global.h"
#include "player.h"
#include "Units/Unit.h"

#ifndef SDL_BASEGAME_ACTIONMENU_H
#define SDL_BASEGAME_ACTIONMENU_H


class ActionMenu {
private:
    Player * _currentPlayer;
    std::vector<std::string> _options;

public:
    void updateOptions(Unit * selected);
};


#endif //SDL_BASEGAME_ACTIONMENU_H
