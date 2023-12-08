#include <fstream>
#include <sstream>
#include <iostream>
#include "Units/Unit.h"
#include "Helper/map_stat_helper.h"
#include "examplegame.h"
#include "Map/Dijkstra.h"
#include "Map/Paths.h"




//std::unique_ptr<Unit> infantryUnit;
std::vector<std::unique_ptr<Unit>> infantryVector;
std::vector<Player> playerVec;

void drawTile(SDL_Renderer *renderer, SDL_Texture *tilesetTexture, int tileIndex, SDL_Rect &destRect, int imgSizeX,
              int tileSize = 16) {
    int tilesPerRow = imgSizeX / tileSize;

    SDL_Rect srcRect;
    srcRect.w = srcRect.h = tileSize;
    srcRect.x = (tileIndex % tilesPerRow) * tileSize;
    srcRect.y = (tileIndex / tilesPerRow) * tileSize;

    destRect.w = destRect.h = 32;

    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
}


void
renderTileset(SDL_Renderer *renderer, SDL_Texture *tileset, TTF_Font *font, SDL_Point imgSize, int tileSize = 16) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    int tilesPerRow = imgSize.x / tileSize;
    int tilesPerColumn = imgSize.y / tileSize;
    srcRect.w = srcRect.h = tileSize;
    destRect.w = destRect.h = tileSize * 2;

    for (int y = 0; y < tilesPerColumn; ++y) {
        for (int x = 0; x < tilesPerRow; ++x) {
            srcRect.x = x * tileSize;
            srcRect.y = y * tileSize;

            destRect.x = x * tileSize * 2;
            destRect.y = y * tileSize * 2;

            SDL_RenderCopy(renderer, tileset, &srcRect, &destRect);

            int index = y * tilesPerRow + x;
            std::string indexText = std::to_string(index);
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, indexText.c_str(), {255, 255, 255, 255});
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect textRect = {destRect.x, destRect.y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

std::vector<std::vector<int>> csvToMap(const string &filename) {
    std::vector<std::vector<int>> map;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
        return map;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }
        map.push_back(row);
    }
    return map;
}


void WarState::Init() {
    uPath.clear();
    RS::getInstance().init(renderer);

    Map.push_back(csvToMap(BasePath "asset/map/pvp/bg.csv"));
    Map.push_back(csvToMap(BasePath"asset/map/pvp/map.csv"));

    pathFinder = new PathFinder(Map, MapStats::getInstance(&Map));
    paths = new Paths(Map, MapStats::getInstance(&Map));

	_texture = IMG_LoadTexture( renderer, BasePath"asset/graphic/NewTiles.png");
    if (!_texture) {
        std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
    }
    RS::getInstance().setTexture( _texture);
    Unit::setTexture( _texture);
	_texture = RS::getInstance().getTexture();


	_panelTexture = IMG_LoadTexture( renderer, BasePath"asset/graphic/panel_beigeLight.png");
	if (!_panelTexture) {
		std::cerr << "Fehler beim Laden der Textur: " << SDL_GetError() << std::endl;
	}
    //infantryUnit = UnitFactory::createUnit(UnitType::INFANTRY, 2, 9, 3);

	_currentTeam = 1;

	infantryVector.push_back(UnitFactory::createUnit(UnitType::INFANTRY, 2, 7,1));
	infantryVector.push_back(UnitFactory::createUnit(UnitType::INFANTRY, 2, 4,1));


	_indexFont = TTF_OpenFont( BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 8);

	SDL_Surface* textSurface = TTF_RenderText_Solid( _indexFont, "def 0", { 0, 0, 0});
	_panelFontTextures.push_back( SDL_CreateTextureFromSurface( renderer, textSurface));
	textSurface = TTF_RenderText_Solid( _indexFont, "def 1", { 0, 0, 0});
	_panelFontTextures.push_back( SDL_CreateTextureFromSurface( renderer, textSurface));
	textSurface = TTF_RenderText_Solid( _indexFont, "def 2", { 0, 0, 0});
	_panelFontTextures.push_back( SDL_CreateTextureFromSurface( renderer, textSurface));
	textSurface = TTF_RenderText_Solid( _indexFont, "def 3", { 0, 0, 0});
	_panelFontTextures.push_back( SDL_CreateTextureFromSurface( renderer, textSurface));
	textSurface = TTF_RenderText_Solid( _indexFont, "def 4", { 0, 0, 0});
	_panelFontTextures.push_back( SDL_CreateTextureFromSurface( renderer, textSurface));

	SDL_FreeSurface(textSurface);

}

void WarState::UnInit() {
    delete (paths);
    delete (pathFinder);

	for(auto k : _panelFontTextures){
		SDL_DestroyTexture(k);
	}
	SDL_DestroyTexture( _panelTexture);
}

bool WarState::HandleEvent(const Event &event) {

    if (event.type == SDL_MOUSEMOTION) {
	    _mousePos.x = event.motion.x;
	    _mousePos.y = event.motion.y;
    }

	if(_unitSelected)
	{
		if( infantryVector[_selectionIndex].get() && paths && paths->mouseInRadius( _mousePos ) )
		{
			uPath = paths->getPath( infantryVector[_selectionIndex]->getCoordinates(), { _mousePos.x / 32, _mousePos.y / 32 }, MovementType::INFANTRY,
			                        3 );

			// MOVE
			if( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT ){
				infantryVector[_selectionIndex]->setCoordinates( _mousePos.x / 32, _mousePos.y / 32 );
			}
		}
		else
		{
			uPath.clear();
		}
		// DESELECT
		if( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT )
		{
			_unitSelected = false;
		}
	}
	else
	{
		if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
			int n = 0;
			for (auto const &unit : infantryVector)
			{
				SDL_Point position = unit->getCoordinates();
				if( (position.x == (_mousePos.x / 32) ) && (position.y == (_mousePos.y / 32)) )
				{
					_selectionIndex = n;
					_unitSelected = true;
				}
				n++;
			}
		}
	}

    return false;
}


void WarState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
}

void WarState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_Rect destRect;
    for (int layer = 0; layer < 2; layer++) {
        for (size_t j = 0; j < Map[layer].size(); j++) {
            for (size_t k = 0; k < Map[layer][j].size(); k++) {
                destRect.x = int(k) * 32;
                destRect.y = int(j) * 32;
                drawTile( renderer, _texture, Map[layer][j][k], destRect, 512);
            }
        }
    }
	if(_unitSelected)
	{
		if(!infantryVector[_selectionIndex]->hasMoved())
		{
			//auto path = paths->getMoveRadius(infantryUnit->getCoordinates(), MovementType::INFANTRY, 3);
			auto path = paths->getMoveRadius( infantryVector[_selectionIndex]->getCoordinates(), MovementType::INFANTRY, 3 );
			paths->drawMoveRadius( frame );
			if( !uPath.empty() )
			{
				for( auto const & point : uPath )
				{
					destRect.x = point.x * 32 + 8;
					destRect.y = point.y * 32 + 8;
					destRect.w = destRect.h = 16;
					SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
					SDL_RenderFillRect( renderer, &destRect );
				}
			}
		}
		for(auto const &unit : infantryVector){
			unit->draw();
		}
	}
	else
	{
		for(auto const &unit : infantryVector){
			unit->draw();
		}

		// draw InfoPanel
		// ----------------------------------------------------------------
		const SDL_Point & winSize = game.GetWindowSize();
		destRect = {10, winSize.y - 60,50, 50};
		SDL_RenderCopy( renderer, _panelTexture, EntireRect, &destRect);

		int defense = 0;
		if( ((_mousePos.x / 32) >= 0 ) && ((_mousePos.y / 32) >= 0)){
			defense = MapStats::getInstance(&Map).getDefense( _mousePos.x / 32, _mousePos.y / 32);
		}
		destRect = {23, winSize.y - 60, 25,15};
		SDL_RenderCopy( renderer, _panelFontTextures[defense], EntireRect, &destRect);
		//--------------------------------------------------------------------
	}

    //renderTileset(renderer, _texture, _indexFont, {512, 208});
}

