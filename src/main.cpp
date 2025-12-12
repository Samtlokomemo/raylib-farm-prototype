#include "raylib.h"
#include "Globals.h"
#include "Player.h"

#include <iostream>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stardew Valley do Paraguai");
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

    Plant plantMap[MAP_HEIGHT][MAP_WIDTH];
    for (int y = 0; y < MAP_HEIGHT; y++){
        for (int x = 0; x < MAP_WIDTH; x++){
            plantMap[y][x].type = NADA;
            plantMap[y][x].age = 0;
            plantMap[y][x].regada = false;
        }
    }

    // Inicializando o player
    Player player{};
    player.x = 1;
    player.y = 1;
    player.dirX = 0;
    player.dirY = 0;
    player.cor = MAROON;
    player.moveTimer = 0.0f;
    player.speed = 0.15f;
    player.itemMao = MAO;
    player.qntSementes = 5;

    const Texture2D tilesetTexture = LoadTexture("assets/tileset.png");
    const Texture2D playerTexture = LoadTexture("assets/tilesetplayer.png");

    Camera2D camera = { 0 };
    camera.target = { static_cast<float>(player.x), static_cast<float>(player.y) };
    camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.zoom = 2.0f;

    printf("SELECIONE A FERRAMENTA: \n 1 - ENXADA\n 2 - REGADOR\n 3 - SEMENTE\n 0 - MAO\n");
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
        const bool colCheck = (alvoX >= 0 && alvoX < MAP_WIDTH && alvoY >= 0 && alvoY < MAP_HEIGHT);
        if ((moveX != 0 || moveY != 0) && player.moveTimer <= 0) {
            // Sistema de colisão e movimento
            if (colCheck) {
                if (const int idTile = map[alvoY][alvoX]; INFO_DOS_TILES[idTile].colisor == false) {
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
        else if (IsKeyPressed(KEY_ZERO)){
            printf("MAO SELECIONADA!\n");
            player.itemMao = MAO;
        }

        // SWITCH DAS FERRAMENTAS
        if (IsKeyPressed(KEY_SPACE)) {
            if (colCheck) {
                int& tileAlvo = map[alvoY][alvoX];
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
                    if (plantMap[alvoY][alvoX].type == NADA && player.qntSementes > 0) {
                        player.qntSementes--;
                        printf("MILHO PLANTADO\n");
                        plantMap[alvoY][alvoX].type = MILHO_SEMENTE;
                    }
                    break;
                default:
                    if (plantMap[alvoY][alvoX].type == MILHO_PRONTO){
                        plantMap[alvoY][alvoX].type = NADA;
                        player.money += 10;
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
                    Plant& p = plantMap[y][x];
                    int& solo = map[y][x];
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
        if (IsKeyPressed(KEY_B)){
            player.money -= 50;
            player.qntSementes += 5;
        }

        camera.target = { static_cast<float>(player.x * TILE_SIZE + TILE_SIZE/2), static_cast<float>(player.y * TILE_SIZE + TILE_SIZE/2)};

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

                const int cursorX = (player.x + player.dirX) * TILE_SIZE;
                const int cursorY = (player.y + player.dirY) * TILE_SIZE;
                DrawRectangleLines(cursorX, cursorY, TILE_SIZE, TILE_SIZE, RED);
            EndMode2D();

            // UI
            DrawText(TextFormat("$ %d", player.money), 10, 10, 20, GREEN);
            DrawText(TextFormat("Sementes Restantes: %d", player.qntSementes), 10, 30, 20, ORANGE);

            const char* nomeFerramenta = "Mão";
            if (player.itemMao == ENXADA) nomeFerramenta = "Enxada";
            if (player.itemMao == REGADOR) nomeFerramenta = "Regador";
            if (player.itemMao == SEMENTE_MILHO) nomeFerramenta = "Semente";
            DrawText(nomeFerramenta, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 20, YELLOW);
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(tilesetTexture);
    CloseWindow();

    return 0;
}