#include "../global.h"
#include "player.h"
#include "Units/Unit.h"
#include <functional>

#ifndef SDL_BASEGAME_ACTIONMENU_H
#define SDL_BASEGAME_ACTIONMENU_H


class ActionMenu {
private:
    Player * _currentPlayer;
    std::vector<std::function<void()>> _options;
public:
    void updateOptions(Unit * selected);
    void generateOptions(Unit * selected, const std::vector<std::vector<Unit*>>& unitMap);
};


#endif //SDL_BASEGAME_ACTIONMENU_H
