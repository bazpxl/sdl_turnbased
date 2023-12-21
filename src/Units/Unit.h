#ifndef SDL_BASEGAME_UNIT_H
#define SDL_BASEGAME_UNIT_H

#include "global.h"
#include "Helper/render_singleton.h"
#include "Helper/enums.h"
#include "CombatCalculator.h"

class CombatCalculator;

class Unit {
protected:
    static SDL_Texture *_texture;
    static std::vector<std::vector<Unit*>> *_unitMap;
    static CombatCalculator *_cc;
    const UnitType _type;
    const MovementType _movementType;
    SDL_Point _coordinates; // Index Position
    int _startX, _startY;
    int _team;
    SDL_RendererFlip _direction = SDL_FLIP_NONE;
    int _hp = 100;
    const int _maxHp = 100;
    const int _moveRange;
    const int _attackRange;
    const int _offset;
    int _fuel;
    const int _maxFuel;
    int _ammo;
    const int _maxAmmo;
    const int _price;
    bool _canAttack = true;
    bool _hasMoved = false;
    bool _hasAttacked = false;
    bool _finishedTurn = false;
    SDL_Renderer *_renderer;

public:
    Unit(UnitType type, MovementType movementType, int x, int y, int startX, int startY, int team, int moveRange,
         int attackRange,int offset, int fuel,
         int maxFuel, int ammo, int maxAmmo, int price, bool canAttack = true);



    virtual ~Unit() = default;

    virtual void draw();

    static void setTexture(SDL_Texture *texture);

    static void setUnitMap(std::vector<std::vector<Unit*>> *unitMap);

    static void setCombatCalculator(CombatCalculator *cc);

    virtual void attack(Unit &other);

    // TODO: Implement specialMove
    //virtual void specialMove() = 0;

    [[nodiscard]] UnitType getType() const;

    [[nodiscard]] MovementType getMovementType() const;

    [[nodiscard]] SDL_Point getCoordinates() const;

    [[nodiscard]] int getHp() const;

    [[nodiscard]] int getTeam() const;

    [[nodiscard]] int getMaxHp() const;

    [[nodiscard]] int getMoveRange() const;

    [[nodiscard]] int getAttackRange() const;

    [[nodiscard]] int getFuel() const;

    [[nodiscard]] int getAmmo() const;

    [[nodiscard]] int getPrice() const;

    [[nodiscard]] bool hasMoved() const;

    [[nodiscard]] bool hasAttacked() const;

    [[nodiscard]] bool isFinishedTurn() const;

    void setCoordinates(int x, int y);

    void setFuel(int fuel);

    void setAmmo(int ammo);

    void setHp(int hp);

    void setHasMoved(bool hasMoved);

    void setHasAttacked(bool hasAttacked);

    void setFinishedTurn(bool finishedTurn);

    void setDirection(SDL_RendererFlip direction);


    void counterAttack(Unit &other);
};

class UnitFactory {
public:
    static std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, int team);
};

class Infantry final : public Unit {
public:
    Infantry(int x, int y, int team);
};

class Mech final : public Unit {
public:
    Mech(int x, int y, int team);
};

class Recon final : public Unit {
public:
    Recon(int x, int y, int team);
};

class Tank final : public Unit {
    Tank(int x, int y, int team);
};

class MediumTank final : public Unit {
    MediumTank(int x, int y, int team);
};

class APC final : public Unit {
    APC(int x, int y, int team);
};

class Artillery final : public Unit {
    Artillery(int x, int y, int team);
};

class Rocket final : public Unit {
    Rocket(int x, int y, int team);
};

class AntiAir final : public Unit {
    AntiAir(int x, int y, int team);
};

class Missiles final : public Unit {
    Missiles(int x, int y, int team);
};

class BattleCopter final : public Unit {
    BattleCopter(int x, int y, int team);
};

class TransportCopter final : public Unit {
    TransportCopter(int x, int y, int team);
};

class Fighter final : public Unit {
    Fighter(int x, int y, int team);
};

class Bomber final : public Unit {
    Bomber(int x, int y, int team);
};

class Lander final : public Unit {
    Lander(int x, int y, int team);
};

class Cruiser final : public Unit {
    Cruiser(int x, int y, int team);
};

class Submarine final : public Unit {
    Submarine(int x, int y, int team);
};

class Battleship final : public Unit {
    Battleship(int x, int y, int team);
};

#endif //SDL_BASEGAME_UNIT_H
