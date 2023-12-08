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
private:
	SDL_Texture* _panelTexture;
	SDL_Texture *_texture;
	TTF_Font *_indexFont;
	std::vector<SDL_Texture*>_panelFontTextures;


	std::vector<SDL_Point> uPath;
	std::vector<std::vector<std::vector<int>>> Map;

	SDL_Point _mousePos;
	bool _unitSelected = false;
	int _selectionIndex;
	int _currentTeam;
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