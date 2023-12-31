#ifndef RENDER_SINGLETON_H
#define RENDER_SINGLETON_H

#include "SDL.h"
#include <vector>
#include "Units/Unit.h"

class Unit;

class RS {
private:
    static RS *_instance;
    SDL_Renderer *_renderer = nullptr;
    SDL_Texture *_mainTexture = nullptr;
    std::vector<std::vector<Unit *>> _unitMap;

    RS() = default;

public:
    static RS &getInstance();

    void init(SDL_Renderer *renderer);

    [[nodiscard]] SDL_Renderer *get() const;

    void setTexture(SDL_Texture *texture);

    [[nodiscard]] SDL_Texture *getTexture() const;

    RS(const RS &) = delete;

    RS &operator=(const RS &) = delete;

    ~RS();

    void setUnitmap(const std::vector<std::vector<Unit *>> &uMap);

    std::vector<std::vector<Unit *>> &getUnitmap();
};

#endif // RENDER_SINGLETON_H
