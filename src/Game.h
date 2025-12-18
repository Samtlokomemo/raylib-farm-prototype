#pragma once
#include "Player.h"
#include "raylib.h"
#include "Globals.h"

struct GameState{
    Player player;
    int map[MAP_HEIGHT][MAP_WIDTH]{};
    Plant plantMap[MAP_HEIGHT][MAP_WIDTH]{};

    Camera2D camera{};

    Texture2D tilesetTexture{};
    Texture2D playerTexture{};
    Sound digSfx{};
    Sound waterupSfx{};
    Sound plantSfx{};

    int shopX{}, shopY{};
    Vector2 cursor{};
};

void InitGame(GameState& game);
void UpdateGame(GameState& game);
void DrawGame(const GameState& game);
void UnloadGame(const GameState& game);
