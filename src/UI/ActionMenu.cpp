#include "ActionMenu.h"

void ActionMenu::generateOptions(Unit* selected, const std::vector<std::vector<Unit*>>& unitMap) {
    SDL_Point coordinates = selected->getCoordinates();

    _options.clear();
    _buttons.clear();

    // Option zum Warten
    _options.emplace_back([selected, this]() {
        selected->setHasMoved(true);
        ActionMenu::_isVisible = false;

    });
    Button waitButton({100,100,300,50}, false, "Warten", _font);
    waitButton.setOnClick(_options.back());
    _buttons.push_back(waitButton);

    int counter = 2;
    std::vector<SDL_Point> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (auto& dir : directions) {
        SDL_Point neighborCoord = {coordinates.x + dir.x, coordinates.y + dir.y};

        if (neighborCoord.x >= 0 && neighborCoord.x < static_cast<int>(unitMap[0].size()) &&
            neighborCoord.y >= 0 && neighborCoord.y < static_cast<int>(unitMap.size())) {

            Unit* neighborUnit = unitMap[neighborCoord.y][neighborCoord.x];
            if (neighborUnit && neighborUnit->getTeam() != selected->getTeam()) {
                std::string attackText = "Angriff auf " + neighborUnit->getTypeName();
                _options.emplace_back([selected, neighborUnit, this]() {
                    selected->attack(*neighborUnit);
                    ActionMenu::_isVisible = false;
                });


                Button attackButton({100,100 * counter,300,50}, false, attackText, _font);
                attackButton.setOnClick(_options.back());
                _buttons.push_back(attackButton);
                counter++;
            }
        }
    }
}

void ActionMenu::drawUI() {
    if(_options.empty() || !_isVisible) {
        return;
    }

    for (auto& button : _buttons) {
        button.render();
    }
}

void ActionMenu::handleEvent(const SDL_Event &e) {
    for (auto& button : _buttons) {
        button.handleEvent(e);
    }
}

ActionMenu::ActionMenu(Player *currentPlayer, TTF_Font *font) : _currentPlayer(currentPlayer), _font(font) {
}


