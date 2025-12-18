#pragma once
#include "raylib.h"
#include "Globals.h"

struct GameState;

// PLAYER
struct Player {
    int x, y;
    int dirX, dirY;
    int moveX, moveY;

    // ECONOMY
    int money;
    int qntSementes;

    float moveTimer;
    float speed;

    ItemType itemMao;
    Color cor;

    // ANIMAÇÃO
    float animTimer;
    int frameIndex;
    bool isMoving;
};

void UpdatePlayer(Player& player, GameState& game, float deltaTime);
void DrawPlayer(const Player& player, Texture2D textura);
