#pragma once
#include "raylib.h"


// CONSTANTES
constexpr int TILE_SIZE = 48;
constexpr int MAP_WIDTH = 11, MAP_HEIGHT = 9;
constexpr int SCREEN_WIDTH = TILE_SIZE * MAP_WIDTH;
constexpr int SCREEN_HEIGHT = TILE_SIZE * MAP_HEIGHT;

// Itens do jogador
enum ItemType {
    MAO,
    ENXADA,
    REGADOR,
    SEMENTE_MILHO
};

// TILES
struct tileInfo {
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

// DATABASE dos Tiles
static constexpr tileInfo INFO_DOS_TILES[]{
    {false, BROWN},
    {false, GREEN},
    {true, DARKBLUE},
    {false, DARKBROWN}
};

// PLANTAS
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

struct Plant{
    PlantType type;
    int age;
    bool regada;
};

static constexpr plantInfo INFO_DAS_PLANTAS[]{
    {false, BLACK},
    {false, LIME},
    {false, ORANGE},
    {false, YELLOW}
};