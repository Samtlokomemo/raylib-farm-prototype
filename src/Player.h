#pragma once
#include "raylib.h"
#include "Globals.h"

// PLAYER
struct Player {
    int x, y;
    int dirX, dirY;

    // ECONOMY
    int money;
    int qntSementes;

    float moveTimer;
    float speed;

    ItemType itemMao;
    Color cor;
};