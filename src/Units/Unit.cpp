#include "Unit.h"

Unit::Unit(UnitType type, MovementType movementType, int x, int y, int startX, int startY, int team, int moveRange,
           int attackRange, int offset, int fuel,
           int maxFuel, int ammo, int maxAmmo, int price, bool canAttack) :
        _type(type),
        _movementType(movementType),
        _coordinates({x, y}),
        _startX(startX),
        _startY(startY),
        _team(team),
        _moveRange(moveRange),
        _attackRange(attackRange),
        _offset(offset),
        _fuel(fuel),
        _maxFuel(maxFuel),
        _ammo(ammo),
        _maxAmmo(maxAmmo),
        _price(price),
        _canAttack(canAttack) {
    _renderer = RS::getInstance().get();

}

void Unit::draw() {
    SDL_Rect sourceRect = {
            _startX,
            _startY + _team * 16,
            16,
            16
    };
    SDL_Rect destRect = {
            _coordinates.x * TILE_SIZE,
            _coordinates.y * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
    };
    if (_finishedTurn || _hasMoved) {
        SDL_SetTextureColorMod(_texture, 128, 128, 128);
    }
    SDL_RenderCopyEx(_renderer, _texture, &sourceRect, &destRect, 0, nullptr, _direction);
    SDL_SetTextureColorMod(_texture, 255, 255, 255);
}

void Unit::showHealth() {
    SDL_Rect barBackground = {
            _coordinates.x * TILE_SIZE,
            _coordinates.y * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE*0.2
    };

    SDL_SetRenderDrawColor(_renderer, 0,0,0,1);
    SDL_RenderFillRect(_renderer, &barBackground);

    SDL_Rect bar = {
        _coordinates.x * TILE_SIZE +2,
        _coordinates.y * TILE_SIZE +2,
        TILE_SIZE*((float)getHp()/(float)getMaxHp()) - 4,
        TILE_SIZE * 0.2 - 4
    };

    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 1);
    SDL_RenderFillRect(_renderer, &bar);
}

int Unit::getTeam() const {
    return _team;
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


void Unit::setDirection(const SDL_RendererFlip direction) {
    _direction = direction;
}

SDL_Point Unit::getCoordinates() const {
    return _coordinates;
}

void Unit::setCoordinates(int x, int y) {
    if (x < _coordinates.x) {
        _direction = SDL_FLIP_HORIZONTAL;
    } else if (x > _coordinates.x) {
        _direction = SDL_FLIP_NONE;
    }
    _coordinates.x = x;
    _coordinates.y = y;
}

SDL_Texture *Unit::_texture = nullptr;

void Unit::setTexture(SDL_Texture *texture) {
    _texture = texture;
}

std::vector<std::vector<Unit *>> *Unit::_unitMap = nullptr;

void Unit::setUnitMap(std::vector<std::vector<Unit *>> *unitMap) {
    _unitMap = unitMap;
}

// Necessary for static member
CombatCalculator *Unit::_cc = nullptr;

void Unit::attack(Unit &other) {
    std::cout << "Attack" << std::endl;
    _hasAttacked = true;
    int dmg = static_cast<int>(_cc->calculateDamage(*this, other));
    std::cout << "Damage: " << dmg << std::endl;
    std::cout << "Attacker: " << this->getHp() << std::endl;
    std::cout << "Defender: " << other.getHp() << std::endl;
    other.setHp(other.getHp() - dmg);
    if (other.getHp() <= 0) {
        _unitMap->at(other.getCoordinates().y).at(other.getCoordinates().x) = nullptr;
        return;
    }

    other.counterAttack(*this);


}

void Unit::counterAttack(Unit &other) {
    std::cout << "Counter Attack" << std::endl;
    int dmg = static_cast<int>(_cc->calculateDamage(other, *this));
    std::cout << "Damage: " << dmg << std::endl;
    std::cout << "Attacker: " << this->getHp() << std::endl;
    std::cout << "Defender: " << other.getHp() << std::endl;
    this->setHp(this->getHp() - dmg);
    if (this->getHp() <= 0) {
        _unitMap->at(this->getCoordinates().y).at(this->getCoordinates().x) = nullptr;
    }
}

void Unit::setCombatCalculator(CombatCalculator *cc) {
    _cc = cc;
}

std::string Unit::getTypeName() const {
    switch (_type) {
        case UnitType::INFANTRY:
            return "Infantry";
        case UnitType::MECH:
            return "Mech";
        case UnitType::RECON:
            return "Recon";
        case UnitType::TANK:
            return "Tank";
        case UnitType::MEDIUM_TANK:
            return "Medium Tank";
        case UnitType::APC:
            return "APC";
        case UnitType::ARTILLERY:
            return "Artillery";
        case UnitType::ROCKET:
            return "Rocket";
        case UnitType::ANTI_AIR:
            return "Anti-Air";
        case UnitType::MISSILE:
            return "Missile";
        case UnitType::BATTLE_COPTER:
            return "Battle Copter";
        case UnitType::TRANSPORT_COPTER:
            return "Transport Copter";
        case UnitType::FIGHTER:
            return "Fighter";
        case UnitType::BOMBER:
            return "Bomber";
        case UnitType::LANDER:
            return "Lander";
        case UnitType::CRUISER:
            return "Cruiser";
        case UnitType::SUBMARINE:
            return "Submarine";
        case UnitType::BATTLESHIP:
            return "Battleship";
        default:
            return "Unknown";
    }
}

Infantry::Infantry(const int x, const int y, const int team) : Unit(UnitType::INFANTRY, MovementType::INFANTRY, x, y,
                                                                    16 * 16, 6 * 16,
                                                                    team, 3, 4, 0, 99, 99, 99,
                                                                    99, 1000) {}

Mech::Mech(const int x, const int y, const int team) : Unit(UnitType::MECH, MovementType::MECH, x, y, 17 * 16, 6 * 16,
                                                            team, 2, 3, 0, 70, 70, 70,
                                                            3, 3000) {}

// ggf anderer TireType
Recon::Recon(const int x, const int y, const int team) : Unit(UnitType::RECON, MovementType::TIRE_A, x, y, 18 * 16,
                                                              6 * 16, team, 8, 1, 0,
                                                              80, 80, 80, 8, 4000) {}

Tank::Tank(int x, int y, int team) : Unit(UnitType::TANK, MovementType::TREAD, x, y, 19 * 16, 6 * 16, team, 6, 1, 0, 99,
                                          99, 99,
                                          9, 7000) {}

MediumTank::MediumTank(int x, int y, int team) : Unit(UnitType::MEDIUM_TANK, MovementType::TREAD, x, y, 20 * 16, 6 * 16,
                                                      team, 5, 1, 0, 70, 70, 70, 8, 12000) {}

APC::APC(int x, int y, int team) : Unit(UnitType::APC, MovementType::TREAD, x, y, 21 * 16, 6 * 16, team, 6, 1, 0, 70,
                                        70, 70, 8,
                                        5000) {}

Artillery::Artillery(int x, int y, int team) : Unit(UnitType::ARTILLERY, MovementType::TREAD, x, y, 22 * 16, 6 * 16,
                                                    team, 5, 2, 0, 99, 99, 99, 9, 6000) {}

Rocket::Rocket(int x, int y, int team) : Unit(UnitType::ROCKET, MovementType::AIR, x, y, 23 * 16, 6 * 16, team, 5, 3, 0,
                                              99, 99,
                                              99, 9, 15000) {}

AntiAir::AntiAir(int x, int y, int team) : Unit(UnitType::ANTI_AIR, MovementType::TREAD, x, y, 24 * 16, 6 * 16, team, 6,
                                                1, 0, 99, 99, 99, 9, 8000) {}

Missiles::Missiles(int x, int y, int team) : Unit(UnitType::MISSILE, MovementType::AIR, x, y, 25 * 16, 6 * 16, team,
                                                  5, 5, 0, 99, 99, 99, 9, 12000) {}

BattleCopter::BattleCopter(int x, int y, int team) : Unit(UnitType::BATTLE_COPTER, MovementType::AIR, x, y, 26 * 16,
                                                          6 * 16, team, 6, 1, 0, 99, 99, 99, 9, 9000) {}

TransportCopter::TransportCopter(int x, int y, int team) : Unit(UnitType::TRANSPORT_COPTER, MovementType::AIR, x, y,
                                                                27 * 16, 6 * 16, team, 6, 0, 0, 99, 99, 99, 9,
                                                                5000) {}

Fighter::Fighter(int x, int y, int team) : Unit(UnitType::FIGHTER, MovementType::AIR, x, y, 28 * 16, 6 * 16, team, 9, 1,
                                                0,
                                                99, 99, 99, 9, 20000) {}

Bomber::Bomber(int x, int y, int team) : Unit(UnitType::BOMBER, MovementType::AIR, x, y, 29 * 16, 6 * 16, team, 7, 1, 0,
                                              99, 99,
                                              99, 9, 22000) {}

Lander::Lander(int x, int y, int team) : Unit(UnitType::LANDER, MovementType::SEA, x, y, 30 * 16, 6 * 16, team, 6, 0, 0,
                                              99, 99,
                                              99, 9, 12000) {}

Cruiser::Cruiser(int x, int y, int team) : Unit(UnitType::CRUISER, MovementType::SEA, x, y, 31 * 16, 6 * 16, team, 5, 0,
                                                0,
                                                99, 99, 99, 9, 18000) {}

Submarine::Submarine(int x, int y, int team) : Unit(UnitType::SUBMARINE, MovementType::SEA, x, y, 32 * 16, 6 * 16, team,
                                                    5, 0, 0, 99, 99, 99, 9, 20000) {}

Battleship::Battleship(int x, int y, int team) : Unit(UnitType::BATTLESHIP, MovementType::SEA, x, y, 33 * 16, 6 * 16,
                                                      team, 5, 0, 0, 99, 99, 99, 9, 28000) {}


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
