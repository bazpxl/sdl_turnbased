#include "../global.h"
#include "player.h"
#include "Units/Unit.h"
#include <functional>

#ifndef SDL_BASEGAME_ACTIONMENU_H
#define SDL_BASEGAME_ACTIONMENU_H


class ActionMenu {
private:
    [[maybe_unused]] Player * _currentPlayer;
    std::vector<std::function<void()>> _options;
    std::vector<SDL_Point> _optionCoordinates;
    std::vector<std::string> _optionTexts;
public:
    bool _isVisible = false;
    void generateOptions(Unit * selected, const std::vector<std::vector<Unit*>>& unitMap);
    void drawUI();
    void selectOption(int index);
};


#endif //SDL_BASEGAME_ACTIONMENU_H
