#include "render_singleton.h"

RS &RS::getInstance() {
    if (_instance == nullptr) {
        _instance = new RS;
    }
    return *_instance;
}

void RS::init(SDL_Renderer *renderer) {
    _renderer = renderer;
}

SDL_Renderer *RS::get() const {
    return _renderer;
}

RS* RS::_instance = nullptr;

