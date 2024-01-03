#include "examplegame.h"
#include "Units/Unit.h"


void ShopState::Init() {
    SDL_Surface* textSurface;
    int abschnitt = 1;
    //Rechtecke für die Einheiten
    for (int i = 0; i < 2; i++)
    {
        unitRecs[i] = (SDL_Rect{ int(p.x * 0.1), int(p.y * double(abschnitt * 0.1)), int(p.x * 0.8), int(p.y * 0.2) });
        abschnitt += 2;
    }

    //Bilder

    imageSurface = IMG_Load(BasePath "asset/graphic/NewTiles.png");
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    croppedTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    font = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", p.y * 0.02);

    // Guthaben
    snprintf(textBuffer, sizeof(textBuffer), "Verfügbares Geld: %d", WarState::currentPlayer->getCurrency());
    textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);
    textTextureGuthaben = SDL_CreateTextureFromSurface(renderer, textSurface);
    guthabenSize = { textSurface->w, textSurface->h };
    SDL_FreeSurface(textSurface);

    for (int i = 0; i < unitRecs.size(); i++)
    {
        snprintf(textBuffer, sizeof(textBuffer), "%s Kosten: %d    HP:%d   Reichweite: %d \nAngriffsreichweite: %d Ammo: %d Fuel: %d",
            unitnames[i], units[i].getPrice(), units[i].getMaxHp(), units[i].getMoveRange(), units[i].getAttackRange(), units[i].getAmmo(), units[i].getFuel());
        textSurface = TTF_RenderText_Solid_Wrapped(font, textBuffer, textColor, 9999);
        textTextures[i] = SDL_CreateTextureFromSurface(renderer, textSurface);
        textureSizes[i] = {textSurface->w, textSurface->h};
        SDL_FreeSurface(textSurface);
    }
}

void ShopState::UnInit() {
    SDL_DestroyTexture(croppedTexture);
    SDL_DestroyTexture(textTextureGuthaben);
    for (SDL_Texture* texture : textTextures) {
        SDL_DestroyTexture(texture);
    }
    TTF_CloseFont(font);
}

bool ShopState::HandleEvent(const Event& event) {

    if (/*SDL_PointInRect(&mousePos, &zurueck ) ||*/ event.type == SDL_KEYDOWN && event.button.button == SDL_SCANCODE_ESCAPE)
    {
        game.SetNextState(0);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        const SDL_Point mousePos = { event.button.x, event.button.y };
        for (auto& buttonRect : unitRecs) {
            if (SDL_PointInRect(&mousePos, &buttonRect))
            {
                std::cout << "halloo" << buttonRect.y <<std::endl;
            }
        }
    }
	return 0;
}

void ShopState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void ShopState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    //Zurück Rechteck
    const SDL_Rect zurueck = SDL_Rect{ int(p.x * 0.9), int(p.y * 0.05), int(p.x * 0.05), int(p.y * 0.05) };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawRect(renderer, &zurueck);
    
    int abschnitt = 1;
    //Farben der Rechtecke und Zeichnen
    for (int i = 0; i < unitRecs.size(); i++)
    {
        if (WarState::currentPlayer->getCurrency() >= prices[i])
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        else
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &unitRecs[i]);
    }
    abschnitt = 1;
    //Bilder Einheiten
    for (int i = 0; i < unitRecs.size(); i++)
    {
        SDL_Rect destRect = { int(p.x * 0.12), int(p.y * double(abschnitt * 0.1) + 0.02), p.y * 0.18, p.y * 0.18 };
        SDL_RenderCopy(renderer, croppedTexture, &einheiten[i], &destRect);
        abschnitt += 2;
    }

    // Guthaben
    SDL_Rect geldrec = { p.x * 0.1, p.y * 0.05, guthabenSize.x, guthabenSize.y };
    SDL_RenderCopy(renderer, textTextureGuthaben, nullptr, &geldrec);

    abschnitt = 1;
    for (int i = 0; i < unitRecs.size(); i++)
    {
        SDL_Rect destRect_ = { p.x * 0.1 + p.y * 0.3, p.y * 0.05 + p.y * (abschnitt * 0.1), textureSizes[i].x, textureSizes[i].y };
        SDL_RenderCopy(renderer, textTextures[i], NULL, &destRect_);

        abschnitt += 2;
    }
    
    
    /*
    int abschnitt = 1;

    //Rechtecke für die Einheiten
    for (int i = 0; i < 2; i++)
    {
        unitRecs[i] = (SDL_Rect{ int(p.x * 0.1), int(p.y * double(abschnitt *0.1)), int(p.x * 0.8), int(p.y * 0.2) });
        abschnitt += 2;
    }
    //Farben der Rechtecke und Zeichnen
    for (int i = 0; i < unitRecs.size(); i++)
    {
        if (WarState::currentPlayer->getCurrency() >= prices[i])
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        else
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &unitRecs[i]);
    }
    //Zurück Rechteck
    zurueck = SDL_Rect{ int(p.x*0.9), int(p.y*0.05), int(p.x*0.05), int(p.y*0.05) };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawRect(renderer, &zurueck);
    //Bilder

    SDL_Surface* imageSurface = IMG_Load(BasePath "asset/graphic/NewTiles.png");
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_Texture* croppedTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    abschnitt = 1;
    for (int i = 0; i < unitRecs.size(); i++)
    {
        SDL_Rect destRect = { int(p.x * 0.12), int(p.y * double(abschnitt * 0.1)+ 0.02), p.y * 0.18, p.y * 0.18 };
        SDL_RenderCopy(renderer, croppedTexture, &einheiten[i], &destRect);
        abschnitt += 2;
    }
    SDL_FreeSurface(imageSurface);
    SDL_DestroyTexture(croppedTexture);
    SDL_Rect heart = { 15 * 16, 16 * 10, 16, 16 };
    //Schrift
    
    char textBuffer[250];
    SDL_Color textColor = { 255, 255, 255, 255 };
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", p.y*0.03);
    snprintf(textBuffer, sizeof(textBuffer), "Verfügbares Geld: %d", WarState::currentPlayer->getCurrency());
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect geldrec = { p.x * 0.1, p.y * 0.05, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &geldrec);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    abschnitt = 1;
    for (int i = 0; i < unitRecs.size(); i++)
    {
        snprintf(textBuffer, sizeof(textBuffer), "%s Kosten: %d    HP:%d   Reichweite: %d",unitnames[i], units[i].getPrice(), units[i].getMaxHp(), units[i].getMoveRange());
        textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect destRect_ = { p.x * 0.1 + p.y * 0.3, p.y * 0.05 + p.y * (abschnitt*0.1), textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &destRect_);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        snprintf(textBuffer, sizeof(textBuffer), "Angriffsreichweite: %d Ammo: %d Fuel: %d", units[i].getAttackRange(), units[i].getAmmo(), units[i].getFuel());
        textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect zweit = { p.x * 0.1 + p.y * 0.3, p.y * 0.1 + p.y * (abschnitt * 0.1), textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &zweit);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        abschnitt += 2;
    }
    
    TTF_CloseFont(font);
    */
	/*    
    int abschnitt = 1;
    //Rechtecke für die Einheiten
    for (int i = 0; i < 2; i++)
    {
        unitRecs[i] = (SDL_Rect{ int(p.x * 0.1), int(p.y * double(abschnitt *0.1)), int(p.x * 0.8), int(p.y * 0.2) });
        abschnitt += 2;
    }
    //Farben der Rechtecke und Zeichnen
    for (int i = 0; i < unitRecs.size(); i++)
    {
        if (WarState::currentPlayer->getCurrency() >= prices[i])
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        else
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &unitRecs[i]);
    }
    //Zurück Rechteck
    zurueck = SDL_Rect{ int(p.x*0.9), int(p.y*0.05), int(p.x*0.05), int(p.y*0.05) };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawRect(renderer, &zurueck);
    //Bilder
    SDL_Surface* imageSurface = IMG_Load(BasePath "asset/graphic/NewTiles.png");
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    SDL_Rect inf = { 16 * 16, 16 * 5, 16, 16 };
    SDL_Rect tank = { 16 * 8, 16 * 5, 16, 16 };
    SDL_Texture* croppedTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_Rect destRect = { int(p.x*0.12), int(p.y*(1*0.1)+0.02), p.y * 0.18, p.y*0.18};
    SDL_Rect destTank = { int(p.x * 0.12), int(p.y * (1 * 0.3) + 0.02), p.y * 0.18, p.y * 0.18 };
    SDL_RenderCopy(renderer, croppedTexture, &inf, &destRect);
    SDL_RenderCopy(renderer, croppedTexture, &tank, &destTank);

    SDL_Rect heart = { 15 * 16, 16 * 10, 16, 16 };
    //Schrift
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", p.y*0.03);
    SDL_Color textColor = { 255, 255, 255, 255 };
    char textBuffer[250];
    snprintf(textBuffer, sizeof(textBuffer), "Infantrie Kosten: %d    HP:%d   Reichweite: %d", infantryCost.getPrice(), infantryCost.getMaxHp(), infantryCost.getMoveRange());
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBuffer, textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect destRect_ = { p.x*0.1+p.y*0.3, p.y*0.1+p.y*0.1, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect_);

    */
}
