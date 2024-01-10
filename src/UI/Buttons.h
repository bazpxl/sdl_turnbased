//
// Created by Jonas.luven on 22/12/2023.
//

#ifndef SDL_BASEGAME_BUTTONS_H
#define SDL_BASEGAME_BUTTONS_H


#include "global.h"
#include <functional>
#include <string>

class Button {
public:


    enum class State {
        NORMAL,
        HOVER,
        PRESSED,
        DISABLED
    };

    Button(SDL_Rect pos, bool shadow, string text, TTF_Font *font);

    ~Button();

    void handleEvent(const SDL_Event& e);
    void render();
    void setPosition(int x, int y);
    void setSize(int w, int h);

    void setOnClick(std::function<void()> callback);

private:
    SDL_Texture* textureNormal;
    SDL_Texture* texturePressed;

    std::string buttonText;
    SDL_Texture* textTexture;
    TTF_Font* buttonFont;

    SDL_Rect rect{};
    State state;
    std::function<void()> onClick;
    [[nodiscard]] bool mouseOverButton(const SDL_Event& e) const;

    SDL_Texture* createTextTexture(SDL_Renderer* renderer, const std::string& text);
    static SDL_Texture* loadTexture(const std::string& path);

    void setText(const string &text);
};

// Implementierungsmethoden...


#endif //SDL_BASEGAME_BUTTONS_H
