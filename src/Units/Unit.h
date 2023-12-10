#ifndef SDL_BASEGAME_UNIT_H
#define SDL_BASEGAME_UNIT_H

#include "global.h"
#include "Helper/render_singleton.h"
#include "Helper/enums.h"


class Unit {
protected:
    static SDL_Texture * _texture;
    const UnitType _type;
    const MovementType _movementType;
    SDL_Point _coordinates; // Index Position
    int _startX, _startY;
    int _team;
public:
	int getTeam() const;
protected:
	SDL_RendererFlip _direction = SDL_FLIP_NONE;
    int _hp = 100;
    const int _maxHp = 100;
    const int _moveRange;
    const int _attackRange;
    int _fuel;
    const int _maxFuel;
    int _ammo;
    const int _price;
    bool _hasMoved = false;
    bool _hasAttacked = false;
    bool _finishedTurn = false;
    SDL_Renderer *_renderer;

public:
    Unit(UnitType type, MovementType movementType, int x, int y, int startX, int startY, int team, int moveRange,
         int attackRange, int fuel,
         int maxFuel, int ammo, int price);

    // TODO: Implement showMovementRange
    //void showMovementRange();

    virtual ~Unit() = default;

    virtual void draw();

    static void setTexture(SDL_Texture *texture);

    // TODO: Implement specialMove
    //virtual void specialMove() = 0;

    [[nodiscard]] UnitType getType() const;

    [[nodiscard]] MovementType getMovementType() const;

    [[nodiscard]] SDL_Point getCoordinates() const;

    [[nodiscard]] int getHp() const;

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


};

class UnitFactory {
public:
    static std::unique_ptr<Unit> createUnit(UnitType type, int x, int y, int team);
};

class Infantry : public Unit {
public:
    Infantry(int x, int y, int team);
};

class Mech : public Unit {
public:
    Mech(int x, int y, int team);
};

#endif //SDL_BASEGAME_UNIT_H
