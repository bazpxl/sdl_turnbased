#include "ActionMenu.h"

void ActionMenu::generateOptions(Unit *selected, const std::vector<std::vector<Unit *>> &unitMap) {
    SDL_Point coordinates = selected->getCoordinates();

    _options.clear();

    _options.emplace_back([selected]() {
        selected->setHasMoved(true);
    });

    std::vector<SDL_Point> directions = {{0,  -1},
                                         {0,  1},
                                         {-1, 0},
                                         {1,  0}};

    for (auto &dir: directions) {
        SDL_Point neighborCoord = {coordinates.x + dir.x, coordinates.y + dir.y};

        if (neighborCoord.x >= 0 && neighborCoord.x < static_cast<int>(unitMap[0].size()) &&
            neighborCoord.y >= 0 && neighborCoord.y < static_cast<int>(unitMap.size())) {

            Unit *neighborUnit = unitMap[neighborCoord.y][neighborCoord.x];

            if (neighborUnit && neighborUnit->getTeam() != selected->getTeam()) {
                _options.emplace_back([selected, neighborUnit]() {
                    selected->attack(*neighborUnit);
                });
            }
        }
    }

}

void ActionMenu::drawUI() {
    if(_options.empty() || !_isVisible) {
        return;
    }
}

void ActionMenu::selectOption(int index) {
    if(index < static_cast<int>(_options.size())) {
        _options[index]();
    }
}
