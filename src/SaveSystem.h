#pragma once
#include <iostream>
#include <fstream>

#include "Game.h"
#include "Globals.h"
#include "Player.h"

inline void SaveGame(const GameState& game){
    FILE* saveFile = fopen("save.dat", "wb");
    if (saveFile == nullptr) printf("O JOGO NÃO CONSEGUIU SALVAR");

    fwrite(&SAVE_VER, sizeof(int), 1, saveFile);
    fwrite(&game.player , sizeof(Player), 1, saveFile);
    fwrite(game.map, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);
    fwrite(game.plantMap, sizeof(Plant) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);

    fclose(saveFile);
    printf("JOGO SALVO COM SUCESSO");
}

inline bool LoadGame(GameState& game){
    FILE* saveFile = fopen("save.dat", "rb");
    if (saveFile == nullptr) printf("O JOGO NÃO CONSEGUIU SALVAR");

    int versaoSave = 0;
    fread(&versaoSave, sizeof(int), 1, saveFile);

    if (versaoSave != SAVE_VER){
        printf("SAVE INCOMPATÍVEL");
        fclose(saveFile);
        return false;
    }
    fread(&game.player, sizeof(Player), 1, saveFile);
    fread(game.map, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);
    fread(game.plantMap, sizeof(Plant) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);

    fclose(saveFile);
    printf("JOGO CARREGADO COM SUCESSO");
    return true;
}