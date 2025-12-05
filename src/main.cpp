#include "raylib.h"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 11, MAP_HEIGHT = 9;

int playerGridX = 1, playerGridY = 1;

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
    while (!WindowShouldClose()) {
        int novoX = -1, novoY = -1;
        // INPUTS
        if (IsKeyPressed(KEY_W)) novoY = playerGridY - 1;
        if (IsKeyPressed(KEY_A)) novoX = playerGridX - 1;
        if (IsKeyPressed(KEY_S)) novoY = playerGridY + 1;
        if (IsKeyPressed(KEY_D)) novoX = playerGridX + 1;

        if (IsKeyPressed(KEY_SPACE)) {
            int espacoAtual = map[playerGridX][playerGridY];
            if (espacoAtual == GRAMA) {
                espacoAtual = TERRA;
            }
            else {
                espacoAtual = GRAMA;
            }
        }

        if (novoX >= 0 && novoX < MAP_WIDTH) {
            int idDoTile = map[playerGridY][novoX];

            if (INFO_DOS_TILES[idDoTile].colisor == false) {
                playerGridX = novoX;
            }
        }

        if (novoY >= 0 && novoY < MAP_HEIGHT) {
            int idDoTile = map[novoY][playerGridX];

            if (INFO_DOS_TILES[idDoTile].colisor == false) {
                playerGridY = novoY;
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

                    DrawRectangle(playerGridX * TILE_SIZE, playerGridY * TILE_SIZE, TILE_SIZE, TILE_SIZE, MAROON);
                }
            }
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}