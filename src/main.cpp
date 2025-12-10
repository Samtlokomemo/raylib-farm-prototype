#include "raylib.h"
#include "Globals.h"
#include "Player.h"

#include <iostream>

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

    int plantMap[MAP_HEIGHT][MAP_WIDTH] = { 0 };

    // Inicializando o player
    Player player;
    player.x = 1;
    player.y = 1;
    player.dirX = 0;
    player.dirY = 0;
    player.cor = MAROON;
    player.moveTimer = 0.0f;
    player.speed = 0.15f;
    player.itemMao = MAO;

    Texture2D tilesetTexture = LoadTexture("assets/tileset.png");
    Texture2D playerTexture = LoadTexture("assets/tilesetplayer.png");

    printf("SELECIONE A FERRAMENTA: \n 1 - ENXADA\n 2 - REGADOR\n 3 - SEMENTE\n");
    // Loop do jogo
    while (!WindowShouldClose()) {

        // Cooldown do movimento
        if (player.moveTimer > 0) {
            player.moveTimer -= GetFrameTime();
        }

        int moveX = 0, moveY = 0;
        
        // INPUTS
        if (player.moveTimer <= 0) {
            if (IsKeyDown(KEY_W)) {
                if (player.dirY != -1) {
                    player.dirX = 0;
                    player.dirY = -1;
                    player.moveTimer = 0.1f;
                }
                else {
                    moveY = -1;
                }
            }
            else if (IsKeyDown(KEY_A)) {
                if (player.dirX != -1) {
                    player.dirX = -1;
                    player.dirY = 0;
                    player.moveTimer = 0.1f;
                }
                else {
                    moveX = -1;
                }
            }
            else if (IsKeyDown(KEY_S)) {
                if (player.dirY != 1) {
                    player.dirX = 0;
                    player.dirY = 1;
                    player.moveTimer = 0.1f;
                }
                else {
                    moveY = 1;
                }
            }
            else if (IsKeyDown(KEY_D)) {
                if (player.dirX != 1) {
                    player.dirX = 1;
                    player.dirY = 0;
                    player.moveTimer = 0.1f;
                }
                else {
                    moveX = 1;
                }
            }
        }
        
        int alvoX = player.x + player.dirX;
        int alvoY = player.y + player.dirY;
        bool colCheck = (alvoX >= 0 && alvoX < MAP_WIDTH && alvoY >= 0 && alvoY < MAP_HEIGHT);
        if ((moveX != 0 || moveY != 0) && player.moveTimer <= 0) {
            // Sistema de colisão e movimento
            if (colCheck) {
                int idTile = map[alvoY][alvoX];

                if (INFO_DOS_TILES[idTile].colisor == false) {
                    player.x = alvoX;
                    player.y = alvoY;

                    player.moveTimer = player.speed;
                }
            }
        }

        // Ferramentas
        if (IsKeyPressed(KEY_ONE)) {
            printf("ENXADA SELECIONADA!\n");
            player.itemMao = ENXADA;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            printf("REGADOR SELECIONADO!\n");
            player.itemMao = REGADOR;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            printf("SEMENTE SELECIONADA!\n");
            player.itemMao = SEMENTE_MILHO;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (colCheck) {
                int& tileAlvo = map[alvoY][alvoX];
                int& plantaAlvo = plantMap[alvoY][alvoX];
                switch (player.itemMao) {
                case ENXADA:
                    if (tileAlvo == GRAMA) {
                        printf("TERRA ARADA!\n");
                        tileAlvo = TERRA;
                    }
                    break;
                case REGADOR:
                    if (tileAlvo == TERRA) {
                        printf("TERRA REGADA!\n");
                        tileAlvo = TERRA_MOLHADA;
                    }
                    break;
                case SEMENTE_MILHO:
                    if (tileAlvo == TERRA_MOLHADA) {
                        if (plantaAlvo == NADA) {
                            printf("MILHO PLANTADO\n");
                            plantaAlvo = MILHO_PRONTO;
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    

                    int tipo = map[y][x];
                    int planta = plantMap[y][x];
                    
                    int posX = x * TILE_SIZE;
                    int posY = y * TILE_SIZE;

                    Vector2 posTela = { (float)posX, (float)posY };

                    Rectangle recorte = {
                        (float)(tipo * TILE_SIZE),
                        0.0f,
                        (float)TILE_SIZE,
                        (float)TILE_SIZE
                    };

                    DrawTextureRec(tilesetTexture, recorte, posTela, WHITE);
                    if (planta != NADA) {
                        DrawCircle(posX + TILE_SIZE / 2, posY + TILE_SIZE / 2, 10, YELLOW);
                    }
                }
            }

            int frameIndex = 0;
            if (player.dirY == -1) frameIndex = 1;
            if (player.dirX == 1)  frameIndex = 2;
            if (player.dirX == -1) frameIndex = 3;
            
            Rectangle playerRec = {
                (float)(frameIndex * TILE_SIZE),
                0.0f,
                (float)TILE_SIZE,
                (float)TILE_SIZE
            };
            Vector2 playerPos = {
                (float)(player.x * TILE_SIZE),
                (float)(player.y * TILE_SIZE)
            };

            DrawTextureRec(playerTexture, playerRec, playerPos, WHITE);

            int cursorX = (player.x + player.dirX) * TILE_SIZE;
            int cursorY = (player.y + player.dirY) * TILE_SIZE;
            DrawRectangleLines(cursorX, cursorY, TILE_SIZE, TILE_SIZE, RED);
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(tilesetTexture);
    CloseWindow();

    return 0;
}