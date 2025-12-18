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
    game.player.moveTimer = 0.0f;
    game.player.speed = 0.15f;
    game.player.itemMao = MAO;
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
        // Cooldown do movimento
        if (game.player.moveTimer > 0) {
            game.player.moveTimer -= GetFrameTime();
        }

        int moveX = 0, moveY = 0;

        game.cursor = {static_cast<float>((game.player.x + game.player.dirX) * TILE_SIZE), static_cast<float>((game.player.y + game.player.dirY) * TILE_SIZE)};

        // INPUTS
        if (game.player.moveTimer <= 0) {
            if (IsKeyDown(KEY_W)) {
                if (game.player.dirY != -1) {
                    game.player.dirX = 0;
                    game.player.dirY = -1;
                    game.player.moveTimer = 0.1f;
                }
                else {
                    moveY = -1;
                }
            }
            else if (IsKeyDown(KEY_A)) {
                if (game.player.dirX != -1) {
                    game.player.dirX = -1;
                    game.player.dirY = 0;
                    game.player.moveTimer = 0.1f;
                }
                else {
                    moveX = -1;
                }
            }
            else if (IsKeyDown(KEY_S)) {
                if (game.player.dirY != 1) {
                    game.player.dirX = 0;
                    game.player.dirY = 1;
                    game.player.moveTimer = 0.1f;
                }
                else {
                    moveY = 1;
                }
            }
            else if (IsKeyDown(KEY_D)) {
                if (game.player.dirX != 1) {
                    game.player.dirX = 1;
                    game.player.dirY = 0;
                    game.player.moveTimer = 0.1f;
                }
                else {
                    moveX = 1;
                }
            }
        }

        int alvoX = game.player.x + game.player.dirX;
        int alvoY = game.player.y + game.player.dirY;
        const bool colCheck = (alvoX >= 0 && alvoX < MAP_WIDTH && alvoY >= 0 && alvoY < MAP_HEIGHT);
        if ((moveX != 0 || moveY != 0) && game.player.moveTimer <= 0) {
            // Sistema de colisão e movimento
            if (colCheck) {
                if (const int idTile = game.map[alvoY][alvoX]; INFO_DOS_TILES[idTile].colisor == false) {
                    game.player.x = alvoX;
                    game.player.y = alvoY;

                    game.player.moveTimer = game.player.speed;
                }
            }
        }

        // Ferramentas
        if (IsKeyPressed(KEY_ONE)) {
            printf("ENXADA SELECIONADA!\n");
            game.player.itemMao = ENXADA;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            printf("REGADOR SELECIONADO!\n");
            game.player.itemMao = REGADOR;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            printf("SEMENTE SELECIONADA!\n");
            game.player.itemMao = SEMENTE_MILHO;
        }
        else if (IsKeyPressed(KEY_ZERO)){
            printf("MAO SELECIONADA!\n");
            game.player.itemMao = MAO;
        }

        // SWITCH DAS FERRAMENTAS
        if (IsKeyPressed(KEY_SPACE)) {
            if (colCheck) {
                int& tileAlvo = game.map[alvoY][alvoX];
                switch (game.player.itemMao) {
                case ENXADA:
                    if (tileAlvo == GRAMA) {
                        PlaySound(game.digSfx);
                        printf("TERRA ARADA!\n");
                        tileAlvo = TERRA;
                    }
                    break;
                case REGADOR:
                    if (tileAlvo == TERRA) {
                        PlaySound(game.waterupSfx);
                        printf("TERRA REGADA!\n");
                        tileAlvo = TERRA_MOLHADA;
                    }
                    break;
                case SEMENTE_MILHO:
                    if (game.plantMap[alvoY][alvoX].type == NADA && game.player.qntSementes > 0 && (tileAlvo == TERRA || tileAlvo == TERRA_MOLHADA)) {
                        PlaySound(game.plantSfx);
                        game.player.qntSementes--;
                        printf("MILHO PLANTADO\n");
                        game.plantMap[alvoY][alvoX].type = MILHO_SEMENTE;
                    }
                    break;
                default:
                    if (game.plantMap[alvoY][alvoX].type == MILHO_PRONTO){
                        game.plantMap[alvoY][alvoX].type = NADA;
                        game.player.money += 10;
                        printf("VOCê GANHOU DINEHRO");
                    }
                    break;
                }
            }
        }

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

        if (IsKeyPressed(KEY_F5)){
            SaveGame(game);
            printf("JOGO SALVO");
        }

        // COMPRAR SEMENTES
        // TODO ARRUMAR AS SEMENTES E A COMPRA DEPOIS PARA O PREÇO SAIR AUTOMATICAMENTE
        if (alvoX == game.shopX && alvoY == game.shopY){
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
                int frameIndex = 0;
                if (game.player.dirY == -1) frameIndex = 1;
                if (game.player.dirX == 1)  frameIndex = 2;
                if (game.player.dirX == -1) frameIndex = 3;

                const Rectangle playerRec = {
                    static_cast<float>(frameIndex * TILE_SIZE),
                    0.0f,
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                };
                const Vector2 playerPos = {
                    static_cast<float>(game.player.x * TILE_SIZE),
                    static_cast<float>(game.player.y * TILE_SIZE)
                };

                DrawTextureRec(game.playerTexture, playerRec, playerPos, WHITE);
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