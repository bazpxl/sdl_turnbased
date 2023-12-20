#include <iostream>
#include "render_singleton.h"

RS *RS::_instance = nullptr;

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

void RS::setTexture(SDL_Texture *texture) {
    if (_mainTexture != nullptr) {
        SDL_DestroyTexture(_mainTexture);
    }
    _mainTexture = texture;
}


SDL_Texture *RS::getTexture() const {
    return _mainTexture;
}

void RS::setUnitmap(const std::vector<std::vector<Unit *>> &uMap) {
    if (uMap.empty()) {
        std::cerr << "Unitmap is empty" << std::endl;
    }
    _unitMap = uMap;
}

std::vector<std::vector<Unit *>> &RS::getUnitmap() {
    return _unitMap;
}


RS::~RS() {
    if (_mainTexture != nullptr) {
        SDL_DestroyTexture(_mainTexture);
    }
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
    }
}

