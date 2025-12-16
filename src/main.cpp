#include "raylib.h"
#include "Globals.h"
#include "Player.h"
#include "SaveSystem.h"

#include <fstream>
#include <iostream>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stardew Valley do Paraguai");
    SetTargetFPS(60);

    InitAudioDevice();
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

    // PLAYER CONFIG
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

    // SHOP CONFIG
    const int shopX = 4;
    const int shopY = 0;
    bool showShopText = false;

    // TEXTURAS

    const Texture2D tilesetTexture = LoadTexture("assets/tileset.png");
    const Texture2D playerTexture = LoadTexture("assets/tilesetplayer.png");

    const Sound digSfx = LoadSound("assets/sfx-dig.wav");
    const Sound waterupSfx = LoadSound("assets/sfx-waterup.wav");
    const Sound plantSfx = LoadSound("assets/sfx-plant.wav");

    Camera2D camera = { 0 };
    camera.target = { static_cast<float>(player.x), static_cast<float>(player.y) };
    camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.zoom = 2.0f;

    printf("SELECIONE A FERRAMENTA: \n 1 - ENXADA\n 2 - REGADOR\n 3 - SEMENTE\n 0 - MAO\n");
    LoadGame(player, *map, *plantMap);
    // Loop do jogo
    while (!WindowShouldClose()) {
        // Cooldown do movimento
        if (player.moveTimer > 0) {
            player.moveTimer -= GetFrameTime();
        }

        int moveX = 0, moveY = 0;

        const Vector2 cursor = {static_cast<float>((player.x + player.dirX) * TILE_SIZE), static_cast<float>((player.y + player.dirY) * TILE_SIZE)};

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
                        PlaySound(digSfx);
                        printf("TERRA ARADA!\n");
                        tileAlvo = TERRA;
                    }
                    break;
                case REGADOR:
                    if (tileAlvo == TERRA) {
                        PlaySound(waterupSfx);
                        printf("TERRA REGADA!\n");
                        tileAlvo = TERRA_MOLHADA;
                    }
                    break;
                case SEMENTE_MILHO:
                    if (plantMap[alvoY][alvoX].type == NADA && player.qntSementes > 0 && (tileAlvo == TERRA || tileAlvo == TERRA_MOLHADA)) {
                        PlaySound(plantSfx);
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

        if (IsKeyPressed(KEY_F5)){
            SaveGame(player, *map, *plantMap);
            printf("JOGO SALVO");
        }

        // COMPRAR SEMENTES
        // TODO ARRUMAR AS SEMENTES E A COMPRA DEPOIS PARA O PREÇO SAIR AUTOMATICAMENTE
        if (alvoX == shopX && alvoY == shopY){
            showShopText = true;
            if (IsKeyPressed(KEY_B) && player.money >= 50){
                player.money -= 50;
                player.qntSementes += 5;
            }
        }else{
            showShopText = false;
        }

        camera.target = { static_cast<float>(player.x * TILE_SIZE + TILE_SIZE/2), static_cast<float>(player.y * TILE_SIZE + TILE_SIZE/2)};
    }

    // De-Initialization
    UnloadSound(digSfx);
    UnloadSound(plantSfx);
    UnloadSound(waterupSfx);
    UnloadTexture(tilesetTexture);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
