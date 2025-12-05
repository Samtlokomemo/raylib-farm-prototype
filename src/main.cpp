#include "raylib.h"
#include <iostream>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 11, MAP_HEIGHT = 9;

int playerGridX = 1, playerGridY = 1;

struct Player {
    int x, y;
    int dirX, dirY;

    float moveTimer;
    float speed;

    Color cor;
};

enum TILES {
    TERRA,
    GRAMA,
    AGUA,

    TILE_COUNT
};

struct tileInfo{
    bool colisor;
    Color cor;
};

const tileInfo INFO_DOS_TILES[]{
    {false, BROWN},
    {false, GREEN},
    {true, DARKBLUE}
};

int main() {
    InitWindow(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE, "Stardew Valley do Paraguai");
    SetTargetFPS(60);
    
    // Matriz dos dados. O MAPA
    // 0 = Terra; 1 = Grama; 2 = ÁGUA
    int map[MAP_HEIGHT][MAP_WIDTH]{
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1}
    };

    Player player;
    player.x = 1;
    player.y = 1;
    player.cor = MAROON;
    player.moveTimer = 0.0f;
    player.speed = 0.15f;

    while (!WindowShouldClose()) {

        if (player.moveTimer > 0) {
            player.moveTimer -= GetFrameTime();
        }

        int moveX = 0, moveY = 0;
        
        // INPUTS
        if (IsKeyDown(KEY_W)) moveY = -1;
        if (IsKeyDown(KEY_A)) moveX = -1;
        if (IsKeyDown(KEY_S)) moveY = 1;
        if (IsKeyDown(KEY_D)) moveX = 1;

        if ((moveX != 0 || moveY != 0) && player.moveTimer <= 0) {
            int alvoX = player.x + moveX;
            int alvoY = player.y + moveY;
            
            if (alvoX >= 0 && alvoX < MAP_WIDTH && alvoY >= 0 && alvoY < MAP_HEIGHT) {
                int idTile = map[alvoY][alvoX];

                if (INFO_DOS_TILES[idTile].colisor == false) {
                    player.x = alvoX;
                    player.y = alvoY;

                    player.moveTimer = player.speed;
                }
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            int& posAtual = map[player.y][player.x];

            if (posAtual == GRAMA) {
                posAtual = TERRA;
            }
            else if (posAtual == TERRA) {
                posAtual = GRAMA;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    int tipo = map[y][x];
                    int posX = x * TILE_SIZE;
                    int posY = y * TILE_SIZE;

                    Color corTile = INFO_DOS_TILES[tipo].cor;

                    // Desenhando os tiles
                    DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, corTile);
                    DrawRectangleLines(posX, posY, TILE_SIZE, TILE_SIZE, DARKGRAY);

                    DrawRectangle(player.x * TILE_SIZE, player.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, player.cor);
                }
            }
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}