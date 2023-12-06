#include "player.h"

Player::Player(int x, int y, int actHouses, int playerTeam, int curr): pos{x,y}, currency{curr}, activeHouses{actHouses}, team{playerTeam} {}

void Player::setPos(const SDL_Point& newPos) {
    _pos = newPos;
}

void Player::setCurrency(int newCurrency) {
    if (newCurrency >= 0){
        _currency = newCurrency;
    }
}

void Player::setActiveHouses(int newActiveHouses) {
    _activeHouses = newActiveHouses;
}

SDL_Point Player::getPos() {
    return _pos;
}

int Player::getCurrency() const{
    return _currency;
}

int Player::getActiveHouses() const{
    return _activeHouses;
}

int Player::getTeam() const{
    return _team;
}


