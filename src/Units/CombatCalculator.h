#ifndef SDL_BASEGAME_COMBATCALCULATOR_H
#define SDL_BASEGAME_COMBATCALCULATOR_H
#include "../Helper/map_stat_helper.h"
#include "Unit.h"

class CombatCalculator {
public:
    explicit CombatCalculator(MapStats &mapStats);
    float calculateDamage(const Unit &attacker, const Unit &defender) const;
private:
    [[maybe_unused]] MapStats &_mapStats;
    std::vector<std::vector<int>> _damageMatrix;
    float getDamage(const Unit &attacker,const Unit &defender) const;
};


#endif //SDL_BASEGAME_COMBATCALCULATOR_H
