#pragma once

#include <global.h>

#include <gamebase.h>
#include "Helper/recthelper.h"
#include "Helper/autocast_ptr.h"
#include "Helper/render_singleton.h"
#include "Map/Dijkstra.h"
#include "Map/Paths.h"
#include "player.h"

class ExampleGame;
class WarState;
class MapState;

class ExampleGame final : public Game
{
public:
	ExampleGame();

	bool HandleEvent( const Event & event ) override;
};

class ExampleGameState : public GameState
{
public:
	[[nodiscard]] ExampleGame & GetGame() const override { return dynamic_cast<ExampleGame &>( GameState::GetGame() ); }

	using GameState::GameState;
};

class WarState : public ExampleGameState
{
public:
    // ctor
    PathFinder *pathFinder;
    Paths *paths;
    Player * currentPlayer;
    using ExampleGameState::ExampleGameState;

    void Init() override;
    void UnInit() override;

    bool HandleEvent( const Event & event ) override;
    void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
    void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

    Color GetClearColor() const override { return Color { 0, 0, 0, SDL_ALPHA_OPAQUE }; }
};

class MapState : public ExampleGameState
{
public:
    // ctor
    using ExampleGameState::ExampleGameState;

    std::vector<std::vector<std::vector<int>>> Map;
    SDL_Texture* texture;

    void drawTile(SDL_Renderer* renderer, SDL_Texture* tilesetTexture, int tileIndex, SDL_Rect& destRect, int imgSizeX, int tileSize);

    void renderTileset(SDL_Renderer* renderer, SDL_Texture* tileset, TTF_Font* font, SDL_Point imgSize, int tileSize);

    void mapToCsv(const std::vector<std::vector<int>> map, const string& filename);

    std::vector<std::vector<int>> csvToMap(const string& filename);
    int selected = 0;
    int currentLayer = 0;
    void Init() override;
    void UnInit() override;

    bool HandleEvent(const Event& event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    Color GetClearColor() const override { return Color{ 0, 0, 0, SDL_ALPHA_OPAQUE }; }
};