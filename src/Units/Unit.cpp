#include "Unit.h"

Unit::Unit(UnitType type, MovementType movementType, int x, int y, int startX, int startY, int team, int moveRange,
           int attackRange, int fuel,
           int maxFuel, int ammo, int price) :
        _type(type),
        _movementType(movementType),
        _coordinates({x, y}),
        _startX(startX),
        _startY(startY),
        _team(team),
        _moveRange(moveRange),
        _attackRange(attackRange),
        _fuel(fuel),
        _maxFuel(maxFuel),
        _ammo(ammo),
        _price(price) {
    _renderer = RS::getInstance().get();
}

void Unit::draw(SDL_Texture *t) {
    SDL_Rect sourceRect = {
            _startX,
            _startY + _team * 16,
            16,
            16
    };
    SDL_Rect destRect = {
            _coordinates.x * 32,
            _coordinates.y * 32,
            32,
            32
    };

    SDL_RenderCopyEx(_renderer, t, &sourceRect, &destRect, 0, nullptr, _direction);
}

UnitType Unit::getType() const {
    return _type;
}

MovementType Unit::getMovementType() const {
    return _movementType;
}

int Unit::getHp() const {
    return _hp;
}

int Unit::getMaxHp() const {
    return _maxHp;
}

int Unit::getMoveRange() const {
    return _moveRange;
}

int Unit::getAttackRange() const {
    return _attackRange;
}

int Unit::getFuel() const {
    return _fuel;
}

int Unit::getAmmo() const {
    return _ammo;
}

int Unit::getPrice() const {
    return _price;
}

bool Unit::hasMoved() const {
    return _hasMoved;
}

bool Unit::hasAttacked() const {
    return _hasAttacked;
}

bool Unit::isFinishedTurn() const {
    return _finishedTurn;
}



void Unit::setFuel(int fuel) {
    _fuel = fuel;
}

void Unit::setAmmo(int ammo) {
    _ammo = ammo;
}

void Unit::setHp(int hp) {
    _hp = hp;
}

void Unit::setHasMoved(bool hasMoved) {
    _hasMoved = hasMoved;
}

void Unit::setHasAttacked(bool hasAttacked) {
    _hasAttacked = hasAttacked;
}

void Unit::setFinishedTurn(bool finishedTurn) {
    _finishedTurn = finishedTurn;
}

void Unit::setDirection(SDL_RendererFlip direction) {
    _direction = direction;
}

SDL_Point Unit::getCoordinates() const {
    return _coordinates;
}

void Unit::setCoordinates(int x, int y) {
    _coordinates.x = x;
    _coordinates.y = y;
}

Infantry::Infantry(int x, int y, int team) : Unit(UnitType::INFANTRY, MovementType::INFANTRY, x, y, 16 * 16, 6 * 16,
                                                  team, 3, 1, 99, 99,
                                                  99, 1000) {}

Mech::Mech(int x, int y, int team) : Unit(UnitType::MECH, MovementType::MECH, x, y, 17 * 16, 6 * 16, team, 2, 1, 70, 70,
                                          3, 3000) {}

std::unique_ptr<Unit> UnitFactory::createUnit(UnitType type, int x, int y, int team) {
    switch (type) {
        case UnitType::INFANTRY:
            return std::make_unique<Infantry>(x, y, team);
        case UnitType::MECH:
            return std::make_unique<Mech>(x, y, team);
        default:
            return nullptr;
    }
}
