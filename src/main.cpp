#include "raylib.h"
#include <iostream>

const int TILE_SIZE = 48;
const int MAP_WIDTH = 11, MAP_HEIGHT = 9;

// Itens do jogador
enum ItemType {
    MAO,
    ENXADA,
    REGADOR,
    SEMENTE_MILHO
};

// CAMADA 0 = TILESET
struct tileInfo{
    bool colisor;
    Color cor;
};

enum TILES {
    TERRA,
    GRAMA,
    AGUA,
    TERRA_MOLHADA,

    TILE_COUNT
};

const tileInfo INFO_DOS_TILES[]{
    {false, BROWN},
    {false, GREEN},
    {true, DARKBLUE},
    {false, DARKBROWN}
};

// CAMADA 1 = PLANTAS E OBJETOS
struct plantInfo {
    bool colisor;
    Color cor;
};

enum PlantType {
    NADA,
    MILHO_SEMENTE,
    MILHO_PEQUENO,
    MILHO_PRONTO
};

const plantInfo INFO_DAS_PLANTAS[]{
    {false, BLACK},
    {false, LIME},
    {false, ORANGE},
    {false, YELLOW}
};

// PLAYER
struct Player {
    int x, y;
    int dirX, dirY;

    float moveTimer;
    float speed;

    ItemType itemMao;
    Color cor;
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

    printf("SELECIONE A FERRAMENTA: \n 1 - ENXADA\n 2 - REGADOR\n 3 - SEMENTE\n");
    // Loop do jogo
    while (!WindowShouldClose()) {

        // Cooldown do movimento
        if (player.moveTimer > 0) {
            player.moveTimer -= GetFrameTime();
        }

        int moveX = 0, moveY = 0;
        
        // INPUTS
        if (IsKeyDown(KEY_W)) { player.dirX = 0; player.dirY = -1; moveY = -1; };
        if (IsKeyDown(KEY_A)) { player.dirX = -1; player.dirY = 0; moveX = -1; };
        if (IsKeyDown(KEY_S)) { player.dirX = 0; player.dirY = 1; moveY = 1; };
        if (IsKeyDown(KEY_D)) { player.dirX = 1; player.dirY = 0; moveX = 1; };

        if (IsKeyPressed(KEY_ONE)) { player.itemMao = ENXADA; printf("ENXADA SELECIONADA!\n"); }
        if (IsKeyPressed(KEY_TWO)) { player.itemMao = REGADOR; printf("REGADOR SELECIONADO!\n"); }
        if (IsKeyPressed(KEY_THREE)) { player.itemMao = SEMENTE_MILHO; printf("SEMENTE SELECIONADA!\n"); }
        
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

                    Color corTile = INFO_DOS_TILES[tipo].cor;
                    Color corPlanta = INFO_DAS_PLANTAS[planta].cor;
                    
                   

                    // Desenhando os tiles
                    DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, corTile);
                    DrawRectangleLines(posX, posY, TILE_SIZE, TILE_SIZE, DARKGRAY);

                    if (planta != NADA) {
                        DrawCircle(posX + TILE_SIZE / 2, posY + TILE_SIZE / 2, 10, YELLOW);
                    }

                }
            }
            DrawRectangle(player.x * TILE_SIZE, player.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, player.cor);
            int cursorX = (player.x + player.dirX) * TILE_SIZE;
            int cursorY = (player.y + player.dirY) * TILE_SIZE;
            DrawRectangleLines(cursorX, cursorY, TILE_SIZE, TILE_SIZE, RED);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}