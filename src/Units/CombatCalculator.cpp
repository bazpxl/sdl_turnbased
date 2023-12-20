#include "CombatCalculator.h"
#include <random>

CombatCalculator::CombatCalculator(MapStats& mapStats) : _mapStats(mapStats) {
    _damageMatrix = MapStats::csvToMatrix(BasePath "asset/calculations/damage.csv");
}

float CombatCalculator::calculateDamage(const Unit& attacker, const Unit& defender) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 9.0);
    const float baseDamage = getDamage(attacker, defender);
    const auto luck = static_cast<float>(dis(gen));
    const auto badLuck = static_cast<float>(dis(gen));
    const float attackerHP = static_cast<float>(attacker.getHp()) / 10.f;
    const auto defenderHP = static_cast<float>(defender.getHp()) / 10.f;
    constexpr float defenseVT = 100.f;
    const auto defenseTl = static_cast<float>(_mapStats.getDefense(defender.getCoordinates()));

    const float damage = (baseDamage + luck - badLuck) * attackerHP / 10 * (
                             (200 - (defenseVT + defenseTl * defenderHP)) / 100.f);

    return damage;
}

float CombatCalculator::getDamage(const Unit& attacker, const Unit& defender) const {
    const int attackerIndex = static_cast<int>(attacker.getType());
    const int defenderIndex = static_cast<int>(defender.getType());
    // Attacker = x axis, Defender = y axis
    return static_cast<float>(_damageMatrix[defenderIndex][attackerIndex]);
}
