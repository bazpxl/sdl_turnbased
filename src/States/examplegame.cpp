#include "examplegame.h"

constexpr bool doBenchmark = true;

ExampleGame::ExampleGame()
	: Game( "Warcrimes", Point{ 1280, 960 }, !doBenchmark )
{
	// Will be freed in Game dtor
	allStates = {
        new WarState        (*this, renderer)
	};

	// The virtual call is ok here
	SetNextState( 0 );

	SetPerfDrawMode( PerformanceDrawMode::Title );
}

bool ExampleGame::HandleEvent( const Event & event )
{
	// Global Keybindings, will be checked before the ones in GameState
	if( Game::HandleEvent( event ) )
		return true;

	switch( event.type )
	{
		case SDL_KEYDOWN:
		{
			const Keysym & what_key = event.key.keysym;

			if( what_key.scancode == SDL_SCANCODE_1 )
			{
				// Both Game and GameState can change the State,
				// but it will only be changed after the current frame
				SetNextState( 0 );
				return true;
			}

			break;
		}

		default:
			break;
	}

	return false;
}
