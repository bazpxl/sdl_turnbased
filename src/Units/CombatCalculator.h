#ifndef SDL_BASEGAME_COMBATCALCULATOR_H
#define SDL_BASEGAME_COMBATCALCULATOR_H
#include "../Helper/map_stat_helper.h"
#include "Unit.h"
#include "global.h"
#include <iostream>
class Unit;
class MapStats;

class CombatCalculator {
public:
    explicit CombatCalculator(MapStats &mapStats);
    [[nodiscard]] float calculateDamage(const Unit &attacker, const Unit &defender) const;
private:
    [[maybe_unused]] MapStats &_mapStats;
    std::vector<std::vector<int>> _damageMatrix;
    [[nodiscard]] float getDamage(const Unit &attacker,const Unit &defender) const;
};


#endif //SDL_BASEGAME_COMBATCALCULATOR_H
