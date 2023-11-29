#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H

#include "global.h"

class Player {
    SDL_Point pos;
    int currency;
    int activeHouses;
    const int team;

private:
    Player(int x, int y, int activeHouses,int team, int currency = 1000);

    void setPos(const SDL_Point &newPos);

    void setCurrency(int newCurrency);

    void setActiveHouses(int newActiveHouses);

    SDL_Point getPos();

    [[nodiscard]] int getCurrency() const;

    [[nodiscard]] int getActiveHouses() const;

    [[nodiscard]] int getTeam() const;
};


#endif
