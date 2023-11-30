#ifndef RH_H
#define RH_H
#include "SDL.h"

class RS {
private:
    SDL_Renderer* _renderer = nullptr;
    static RS* _instance;

    RS() = default;

public:
    static RS& getInstance();

    void init(SDL_Renderer* renderer);

    [[nodiscard]] SDL_Renderer* get() const;

    // Prevent copy and assignment
    RS(RS const&) = delete;
    void operator=(RS const&)  = delete;
};




#endif