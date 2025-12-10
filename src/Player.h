#pragma once
#include "raylib.h"
#include "Globals.h"

// PLAYER
struct Player {
    int x, y;
    int dirX, dirY;

    float moveTimer;
    float speed;

    ItemType itemMao;
    Color cor;
};