#include "player.h"

Player::Player(int x, int y, int actHouses, int playerTeam, int curr): pos{x,y}, currency{curr}, activeHouses{actHouses}, team{playerTeam} {}

void Player::setPos(const SDL_Point& newPos) {
    pos = newPos;
}

void Player::setCurrency(int newCurrency) {
    if (newCurrency >= 0){
        currency = newCurrency;
    }
}

void Player::setActiveHouses(int newActiveHouses) {
    activeHouses = newActiveHouses;
}

SDL_Point Player::getPos() {
    return pos;
}

int Player::getCurrency() const{
    return currency;
}

int Player::getActiveHouses() const{
    return activeHouses;
}

int Player::getTeam() const{
    return team;
}


