#include "examplegame.h"
#include "Units/Unit.h"
Infantry infantryCost(3,2,3);
SDL_Rect zurueck;
Point p = {15 * 32, 11 * 32};
Array<SDL_Rect,2> unitRecs;
Array<int, 2> prices = {1000, 3000};
Array<SDL_Rect, 2> einheiten = { 16 * 16, 16 * 5, 16, 16, 17 * 16, 16 * 5, 16, 16 };
Array<Unit, 2> units = {Infantry(3,2,1), Mech(4,5,1)};
Array<char[20], 2> unitnames = {"Infantry", "Mech"};

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

    if (SDL_PointInRect(&mousePos, &zurueck ) || event.type == SDL_KEYDOWN && event.button.button == SDL_SCANCODE_ESCAPE)
    {
        game.SetNextState(0);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {

    }
	return 0;
}

void ShopState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void ShopState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
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
