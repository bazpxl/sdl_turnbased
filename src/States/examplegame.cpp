#include "examplegame.h"

[[maybe_unused]] constexpr bool doBenchmark = true;

SDL_Point getGameDimensions(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;
    int row = 0;
    int col = 0;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        int currentColCount = 0;

        while (std::getline(lineStream, cell, ',')) {
            currentColCount++;
        }

        if (row == 0) {
            col = currentColCount;
        }

        row++;
    }

    return {col * TILE_SIZE, row * TILE_SIZE};
}

ExampleGame::ExampleGame()
        : Game("Warcrimes",getGameDimensions(BasePath "asset/map/pvp/bg.csv"), false) {
    // Will be freed in Game dtor
    allStates = {
            new WarState    (*this, renderer),
            new ShopState	(*this, renderer),
            new MapState(*this, renderer)
    };

    // The virtual call is ok here
    SetNextState(0);

    SetPerfDrawMode(PerformanceDrawMode::Title);
}

bool ExampleGame::HandleEvent(const Event &event) {
    // Global Keybindings, will be checked before the ones in GameState
    if (Game::HandleEvent(event))
        return true;

    switch (event.type) {
        case SDL_KEYDOWN: {
            const Keysym &what_key = event.key.keysym;

            if (what_key.scancode == SDL_SCANCODE_1) {
                // Both Game and GameState can change the State,
                // but it will only be changed after the current frame
                SetNextState(0);
                return true;
            }

			else if (what_key.scancode == SDL_SCANCODE_2)
			{
				SetNextState(1);
				return true;
			}

			break;
		}
            else if (what_key.scancode == SDL_SCANCODE_3) {
                // Both Game and GameState can change the State,
                // but it will only be changed after the current frame
                SetNextState(2);
                return true;
            }
            break;
        }

        default:
            break;
    }

    return false;
}
