#include "Player.h"
#include "Game.h"
#include <iostream>

void UpdatePlayer(Player& player, GameState& game, float deltaTime)
{
    player.moveX = 0, player.moveY = 0;

    // Cooldown do movimento
    if (player.moveTimer > 0) {
        player.moveTimer -= GetFrameTime();
    }

    // INPUTS
    if (player.moveTimer <= 0) {
        if (IsKeyDown(KEY_W)) {
            if (player.dirY != -1) {
                player.dirX = 0;
                player.dirY = -1;
                player.moveTimer = 0.1f;
                player.isMoving = true;
            }
            else {
                player.moveY = -1;
            }
        }
        else if (IsKeyDown(KEY_A)) {
            if (player.dirX != -1) {
                player.dirX = -1;
                player.dirY = 0;
                player.moveTimer = 0.1f;
                player.isMoving = true;
            }
            else {
                player.moveX = -1;
            }
        }
        else if (IsKeyDown(KEY_S)) {
            if (player.dirY != 1) {
                player.dirX = 0;
                player.dirY = 1;
                player.moveTimer = 0.1f;
                player.isMoving = true;
            }
            else {
                player.moveY = 1;
            }
        }
        else if (IsKeyDown(KEY_D)) {
            if (player.dirX != 1) {
                player.dirX = 1;
                player.dirY = 0;
                player.moveTimer = 0.1f;
                player.isMoving = true;
            }
            else {
                player.moveX = 1;
            }
        }else{
            player.isMoving = false;
        }
    }

    int alvoX = player.x + player.dirX;
    int alvoY = player.y + player.dirY;
    const bool colCheck = (alvoX >= 0 && alvoX < MAP_WIDTH && alvoY >= 0 && alvoY < MAP_HEIGHT);
    if ((player.moveX != 0 || player.moveY != 0) && player.moveTimer <= 0) {
        // Sistema de colisão e movimento
        if (colCheck) {
            if (const int idTile = game.map[alvoY][alvoX]; INFO_DOS_TILES[idTile].colisor == false) {
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
            int& tileAlvo = game.map[alvoY][alvoX];
            switch (player.itemMao) {
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
                if (game.plantMap[alvoY][alvoX].type == NADA && player.qntSementes > 0 && (tileAlvo == TERRA || tileAlvo == TERRA_MOLHADA)) {
                    PlaySound(game.plantSfx);
                    player.qntSementes--;
                    printf("MILHO PLANTADO\n");
                    game.plantMap[alvoY][alvoX].type = MILHO_SEMENTE;
                }
                break;
            default:
                if (game.plantMap[alvoY][alvoX].type == MILHO_PRONTO){
                    game.plantMap[alvoY][alvoX].type = NADA;
                    player.money += 10;
                    printf("VOCê GANHOU DINEHRO");
                }
                break;
            }
        }
    }

    // ANIMAÇÃO
    if (player.frameIndex > 3) player.frameIndex = 0;
    if (player.isMoving){
        player.animTimer += deltaTime;
    }else{
        player.frameIndex = 0;
    }
    if (player.animTimer >= 1){
        player.animTimer = 0;
        player.frameIndex += 1;
    }
}

void DrawPlayer(const Player& player, Texture2D textura)
{
    int animIndex = 0;
    if (player.dirY == -1) animIndex = 1;
    if (player.dirX == 1)  animIndex = 2;
    if (player.dirX == -1) animIndex = 3;

    const Rectangle playerRec = {
        static_cast<float>(player.frameIndex * TILE_SIZE),
        static_cast<float>(animIndex * TILE_SIZE),
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    };
    const Vector2 playerPos = {
        static_cast<float>(player.x * TILE_SIZE),
        static_cast<float>(player.y * TILE_SIZE)
    };

    DrawTextureRec(textura, playerRec, playerPos, WHITE);
}

