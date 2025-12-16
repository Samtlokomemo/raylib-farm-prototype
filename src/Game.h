#pragma once
#include "Player.h"
#include "Raylib.h"
#include "Globals.h"
//TODO FAZER AS FUNÇÕES

inline void UpdateGame(Player player, int* map, Plant* plantMap){

 }

inline void DrawGame(const Player& player, int* map, Plant* plantMap, const Camera2D& camera){
          BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
                for (int y = 0; y < MAP_HEIGHT; y++)
                {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        const int tipo = map[y][x];

                        const int posX = x * TILE_SIZE;
                        const int posY = y * TILE_SIZE;

                        const Vector2 posTela = { static_cast<float>(posX), static_cast<float>(posY) };

                        const Rectangle recorte = {
                            static_cast<float>(tipo * TILE_SIZE),
                            0.0f,
                            static_cast<float>(TILE_SIZE),
                            static_cast<float>(TILE_SIZE)
                        };

                        DrawTextureRec(tilesetTexture, recorte, posTela, WHITE);

                        // DESENHANDO AS PLANTAS
                        if (int tipoPlanta = plantMap[y][x].type; tipoPlanta != NADA) {
                            const Rectangle planta = {
                                static_cast<float>((tipoPlanta-1) * TILE_SIZE),
                                48.0f,
                                static_cast<float>(TILE_SIZE),
                                static_cast<float>(TILE_SIZE)
                            };
                            DrawTextureRec(tilesetTexture, planta, posTela, WHITE);
                            //DrawCircle(posX + TILE_SIZE / 2, posY + TILE_SIZE / 2, 10, INFO_DAS_PLANTAS[tipoPlanta].cor);
                        }
                    }
                }

                // PLAYER RENDER
                int frameIndex = 0;
                if (player.dirY == -1) frameIndex = 1;
                if (player.dirX == 1)  frameIndex = 2;
                if (player.dirX == -1) frameIndex = 3;

                const Rectangle playerRec = {
                    static_cast<float>(frameIndex * TILE_SIZE),
                    0.0f,
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                };
                const Vector2 playerPos = {
                    static_cast<float>(player.x * TILE_SIZE),
                    static_cast<float>(player.y * TILE_SIZE)
                };

                DrawTextureRec(playerTexture, playerRec, playerPos, WHITE);
                DrawRectangleLines(static_cast<int>(cursor.x), static_cast<int>(cursor.y), TILE_SIZE, TILE_SIZE, RED);

                // LOJA RENDER
                const Rectangle shopRec = {
                    0.0f,
                    2.0f * TILE_SIZE,
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                };
                DrawTextureRec(tilesetTexture, shopRec, {static_cast<float>(shopX * TILE_SIZE), static_cast<float>(shopY)}, WHITE);
            EndMode2D();

            // UI
            DrawText(TextFormat("$ %d", player.money), 10, 10, 20, GREEN);
            DrawText(TextFormat("Sementes Restantes: %d", player.qntSementes), 10, 30, 20, ORANGE);

            if (showShopText){
                DrawText("aperte B para comprar", SCREEN_WIDTH / 2 - 120, 60, 20, WHITE);
            }

            const char* nomeFerramenta = "Mão";
            if (player.itemMao == ENXADA) nomeFerramenta = "Enxada";
            if (player.itemMao == REGADOR) nomeFerramenta = "Regador";
            if (player.itemMao == SEMENTE_MILHO) nomeFerramenta = "Semente";
            DrawText(nomeFerramenta, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 20, YELLOW);
        EndDrawing();
}
