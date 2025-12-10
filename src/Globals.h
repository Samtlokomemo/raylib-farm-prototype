#pragma once
#include "raylib.h"

// CONSTANTES
const int TILE_SIZE = 48;
const int MAP_WIDTH = 11, MAP_HEIGHT = 9;

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
static const tileInfo INFO_DOS_TILES[]{
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

static const plantInfo INFO_DAS_PLANTAS[]{
    {false, BLACK},
    {false, LIME},
    {false, ORANGE},
    {false, YELLOW}
};