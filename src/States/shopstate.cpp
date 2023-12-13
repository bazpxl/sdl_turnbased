#include "examplegame.h"
#include "Units/Unit.h"
Infantry infantryCost(3,2,3);
SDL_Rect zurueck;
void ShopState::Init() {
	//Infantry InfantryCost = UnitFactory::createUnit(UnitType::INFANTRY, 7, 10, 3);

}

void ShopState::UnInit() {

}

bool ShopState::HandleEvent(const Event& event) {
    SDL_Point mousePos;
    if (event.type == SDL_MOUSEMOTION) {
        mousePos.x = event.motion.x;
        mousePos.y = event.motion.y;
    }
    if (SDL_PointInRect(&mousePos, &zurueck)) {
        game.SetNextState(0);
    }

	return 0;
}

void ShopState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void ShopState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_Rect infrec = SDL_Rect{ 100, 80, 728, 80 };
	SDL_RenderDrawRect(renderer, &infrec);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_Rect other = SDL_Rect{ 100, 170, 728, 80 };
	SDL_RenderDrawRect(renderer, &other);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    zurueck = SDL_Rect{ 828, 50, 30, 30 };
    SDL_RenderDrawRect(renderer, &zurueck);


    SDL_Surface* imageSurface = IMG_Load(BasePath "asset/graphic/NewTiles.png");
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_Rect sourceRect = { 16*16, 16*5, 16, 16 };
    SDL_Texture* croppedTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_Rect destRect = { 105, 85, 70, 70 };

    SDL_RenderCopy(renderer, croppedTexture, &sourceRect, &destRect);


    SDL_Rect heart = { 15 * 16, 16 * 10, 16, 16 };
    
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 20);
    SDL_Color textColor = { 255, 255, 255, 255 };
    char textBuffer[50]; 
    snprintf(textBuffer, sizeof(textBuffer), "Infantrie Kosten: %d    HP:%d   Reichweite: %d", infantryCost.getPrice(), infantryCost.getMaxHp(), infantryCost.getMoveRange());
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    SDL_Rect destRect_ = { 200, 110, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect_);


}