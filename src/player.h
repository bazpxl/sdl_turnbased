#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H

#include "global.h"

class Player {
    SDL_Point _pos;
    int _currency;
    int _activeHouses;
    const int _team;

private:

	void setPos(const SDL_Point &newPos);

    void setCurrency(int newCurrency);

    void setActiveHouses(int newActiveHouses);

    SDL_Point getPos();

    [[nodiscard]] int getCurrency() const;

    [[nodiscard]] int getActiveHouses() const;

public:
	Player(int x, int y, int activeHouses,int team, int currency = 1000);
	[[nodiscard]] int getTeam() const;
};


#endif
