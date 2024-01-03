#include "../global.h"
#include "player.h"
#include "Units/Unit.h"
#include "Buttons.h"
#include <functional>

#ifndef SDL_BASEGAME_ACTIONMENU_H
#define SDL_BASEGAME_ACTIONMENU_H


class ActionMenu {
private:
    std::vector<std::function<void()>> _options;
    std::vector<std::unique_ptr<Button>> _buttons;
    TTF_Font* _font;


public:

    ActionMenu() :  _font(nullptr) {}
    explicit ActionMenu(TTF_Font* font);
    bool _isVisible = false;
    void generateOptions(Unit * selected, const std::vector<std::vector<Unit*>>& unitMap);
    void drawUI();
    void handleEvent(const SDL_Event& e);
    void Test();
};


#endif //SDL_BASEGAME_ACTIONMENU_H
