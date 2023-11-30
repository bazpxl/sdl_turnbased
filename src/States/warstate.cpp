#include "examplegame.h"

void WarState::Init() {
    GameState::Init();
}

void WarState::UnInit() {
    GameState::UnInit();
}

bool WarState::HandleEvent(const Event &event) {
    return false;
}

void WarState::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void WarState::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}
