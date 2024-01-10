//
// Created by Jonas.luven on 22/12/2023.
//

#include "Buttons.h"

#include <utility>
#include "Helper/render_singleton.h"


Button::Button(SDL_Rect pos, bool shadow, std::string text, TTF_Font *font) : buttonText(std::move(text)), textTexture(nullptr),
                                                                                     buttonFont(font) {
    rect = pos;
    state = State::NORMAL;
    setText(buttonText);

    if (shadow) {
        textureNormal = loadTexture(BasePath"asset/graphic/UI/PNG/reflection_button_up.png");
        texturePressed = loadTexture(BasePath"asset/graphic/UI/PNG/reflection_button_down.png");
    } else {
        textureNormal = loadTexture(BasePath"asset/graphic/UI/PNG/no_reflection_button_up.png");
        texturePressed = loadTexture(BasePath"asset/graphic/UI/PNG/no_reflection_button_down.png");
    }
}

SDL_Texture *Button::loadTexture(const std::string &path) {
    return IMG_LoadTexture(RS::getInstance().get(), path.c_str());
}


Button::~Button() {
    SDL_DestroyTexture(textureNormal);
    SDL_DestroyTexture(texturePressed);
}

void Button::handleEvent(const SDL_Event &e) {
    bool isMouseOver = mouseOverButton(e);

    switch (e.type) {
        case SDL_MOUSEMOTION:
            state = isMouseOver ? State::HOVER : State::NORMAL;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT && isMouseOver) {
                state = State::PRESSED;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                state = isMouseOver ? State::HOVER : State::NORMAL;
                if (isMouseOver && onClick) {
                    onClick();
                }
            }
            break;

        default:
            break;
    }
}

bool Button::mouseOverButton(const SDL_Event &e) const {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void Button::setText(const std::string &text) {
    SDL_DestroyTexture(textTexture);
    textTexture = createTextTexture(RS::getInstance().get(), text);
}


void Button::render() {
    // Hintergrund des Buttons rendern
    SDL_Texture* currentTexture = nullptr;
    switch (state) {
        case State::NORMAL:
            SDL_SetTextureColorMod(currentTexture, 217, 163, 134);
            currentTexture = textureNormal;
            break;
        case State::PRESSED:
            SDL_SetTextureColorMod(texturePressed, 255, 0, 0);
            currentTexture = texturePressed;
            break;
        case State::HOVER:
            SDL_SetTextureColorMod(textureNormal, 255, 0, 0);
            currentTexture = textureNormal;
            break;
        case State::DISABLED:
            // Optional: Behandlung des deaktivierten Zustands
            break;
    }
    if (currentTexture) {
        SDL_RenderCopy(RS::getInstance().get(), currentTexture, nullptr, &rect);
        SDL_SetTextureColorMod(currentTexture, 217, 163, 134);
    }

    if (textTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

        // Offset vom Rand
        const int offset = 15;

        float scaleX = static_cast<float>(rect.w - 2 * offset) / textWidth;
        float scaleY = static_cast<float>(rect.h - 2 * offset) / textHeight;
        float scale = std::min(scaleX, scaleY);

        int newTextWidth = static_cast<int>(textWidth * scale);
        int newTextHeight = static_cast<int>(textHeight * scale);

        SDL_Rect textRect = {
                rect.x + (rect.w - newTextWidth) / 2,
                rect.y + (rect.h - newTextHeight) / 2,
                newTextWidth,
                newTextHeight
        };
        SDL_RenderCopy(RS::getInstance().get(), textTexture, nullptr, &textRect);
    }
}



void Button::setOnClick(std::function<void()> callback) {
    onClick = std::move(callback);
}

SDL_Texture *Button::createTextTexture(SDL_Renderer *renderer, const string &text) {
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended(buttonFont, text.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return texture;
}

