#include "Game.h"
#include "SaveSystem.h"

#include <iostream>

void InitGame(GameState& game){
    // Matriz dos dados. O MAPA
    // 0 = Terra; 1 = Grama; 2 = ÁGUA
    static const int MapaBase[MAP_HEIGHT][MAP_WIDTH] = {
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
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            game.map[y][x] = MapaBase[y][x];
        }
    };

    for (auto & y : game.plantMap){
        for (auto & [type, age, regada] : y){
            type = NADA;
            age = 0;
            regada = false;
        }
    }

    // PLAYER CONFIG
    game.player.x = 1;
    game.player.y = 1;
    game.player.dirX = 0;
    game.player.dirY = 0;
    game.player.cor = MAROON;
    // PLAYER MOVIMENTAÇÃO
    game.player.moveTimer = 0.0f;
    game.player.speed = 0.15f;
    game.player.isMoving = false;
    // PLAYER INVENTÁRIO
    game.player.itemMao = MAO;
    game.player.money = 0;
    game.player.qntSementes = 5;

    // SPRITES
    game.tilesetTexture = LoadTexture("assets/tileset.png");
    game.playerTexture = LoadTexture("assets/tilesetplayer.png");

    game.shopX = 4;
    game.shopY = 0;

    game.digSfx = LoadSound("assets/sfx-dig.wav");
    game.waterupSfx = LoadSound("assets/sfx-waterup.wav");
    game.plantSfx = LoadSound("assets/sfx-plant.wav");

    game.camera.target = { static_cast<float>(game.player.x), static_cast<float>(game.player.y) };
    game.camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    game.camera.zoom = 2.0f;

    printf("SELECIONE A FERRAMENTA: \n 1 - ENXADA\n 2 - REGADOR\n 3 - SEMENTE\n 0 - MAO\n");
    LoadGame(game);
}

void UpdateGame(GameState& game){
    game.cursor = {static_cast<float>((game.player.x + game.player.dirX) * TILE_SIZE), static_cast<float>((game.player.y + game.player.dirY) * TILE_SIZE)};

    if (IsKeyPressed(KEY_F5)){
        SaveGame(game);
        printf("JOGO SALVO");
    }

    UpdatePlayer(game.player, game, GetFrameTime());

    // Passar o dia
    if (IsKeyPressed(KEY_ENTER)){
        printf("UM NOVO DIA CHEGOU!");
        for (int y = 0; y < MAP_HEIGHT; y++){
            for (int x = 0; x < MAP_WIDTH; x++){
                Plant& p = game.plantMap[y][x];
                int& solo = game.map[y][x];
                if (p.type != NADA && solo == TERRA_MOLHADA){
                    p.age++;
                    if (p.type == MILHO_SEMENTE && p.age >= 1){
                        p.type = MILHO_PEQUENO;
                    }else if (p.type == MILHO_PEQUENO && p.age >= 3){
                        p.type = MILHO_PRONTO;
                    }
                }
                if (solo == TERRA_MOLHADA){
                    solo = TERRA;
                }
            }
        }
    }

    // COMPRAR SEMENTES
    // TODO ARRUMAR AS SEMENTES E A COMPRA DEPOIS PARA O PREÇO SAIR AUTOMATICAMENTE
    if (game.cursor.x == game.shopX && game.cursor.y == game.shopY){
        if (IsKeyPressed(KEY_B) && game.player.money >= 50){
            game.player.money -= 50;
            game.player.qntSementes += 5;
        }
    }

    game.camera.target = { static_cast<float>(game.player.x * TILE_SIZE), static_cast<float>(game.player.y * TILE_SIZE )};
}

void DrawGame(const GameState& game){
    BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(game.camera);
                for (int y = 0; y < MAP_HEIGHT; y++)
                {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        const int tipo = game.map[y][x];

                        const int posX = x * TILE_SIZE;
                        const int posY = y * TILE_SIZE;

                        const Vector2 posTela = { static_cast<float>(posX), static_cast<float>(posY) };

                        const Rectangle recorte = {
                            static_cast<float>(tipo * TILE_SIZE),
                            0.0f,
                            static_cast<float>(TILE_SIZE),
                            static_cast<float>(TILE_SIZE)
                        };

                        DrawTextureRec(game.tilesetTexture, recorte, posTela, WHITE);

                        // DESENHANDO AS PLANTAS
                        if (int tipoPlanta = game.plantMap[y][x].type; tipoPlanta != NADA) {
                            const Rectangle planta = {
                                static_cast<float>((tipoPlanta-1) * TILE_SIZE),
                                48.0f,
                                static_cast<float>(TILE_SIZE),
                                static_cast<float>(TILE_SIZE)
                            };
                            DrawTextureRec(game.tilesetTexture, planta, posTela, WHITE);
                        }
                    }
                }

                // PLAYER RENDER
                DrawPlayer(game.player, game.playerTexture);
                DrawRectangleLines(static_cast<int>(game.cursor.x), static_cast<int>(game.cursor.y), TILE_SIZE, TILE_SIZE, RED);

                // LOJA RENDER
                constexpr Rectangle shopRec = {
                    0.0f,
                    2.0f * TILE_SIZE,
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                };
                DrawTextureRec(game.tilesetTexture, shopRec, {static_cast<float>(game.shopX * TILE_SIZE), static_cast<float>(game.shopY)}, WHITE);
            EndMode2D();

            // UI
            DrawText(TextFormat("$ %d", game.player.money), 10, 10, 20, GREEN);
            DrawText(TextFormat("Sementes Restantes: %d", game.player.qntSementes), 10, 30, 20, ORANGE);

            if (game.cursor.x == static_cast<float>(game.shopX) && game.cursor.y == static_cast<float>(game.shopY)){
                printf("aaa");
                DrawText("aperte B para comprar", SCREEN_WIDTH / 2 - 120, 60, 20, WHITE);
            }

            const char* nomeFerramenta = "Mão";
            if (game.player.itemMao == ENXADA) nomeFerramenta = "Enxada";
            if (game.player.itemMao == REGADOR) nomeFerramenta = "Regador";
            if (game.player.itemMao == SEMENTE_MILHO) nomeFerramenta = "Semente";
            DrawText(nomeFerramenta, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 20, YELLOW);
        EndDrawing();
}

void UnloadGame(const GameState& game){
    UnloadSound(game.digSfx);
    UnloadSound(game.plantSfx);
    UnloadSound(game.waterupSfx);
    UnloadTexture(game.tilesetTexture);
}