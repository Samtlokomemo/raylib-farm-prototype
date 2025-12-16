#pragma once
#include <iostream>
#include <fstream>
#include "Globals.h"
#include "Player.h"

inline void SaveGame(const Player& player, const int* sMap, const Plant* sPlantMap){
    FILE* saveFile = fopen("save.dat", "wb");
    if (saveFile == nullptr) printf("O JOGO NÃO CONSEGUIU SALVAR");

    fwrite(&player , sizeof(Player), 1, saveFile);
    fwrite(sMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);
    fwrite(sPlantMap, sizeof(Plant) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);

    fclose(saveFile);
    printf("JOGO SALVO COM SUCESSO");
}

inline void LoadGame(Player& sPlayer, int* sMap, Plant* sPlantMap){
    FILE* saveFile = fopen("save.dat", "rb");
    if (saveFile == nullptr) printf("O JOGO NÃO CONSEGUIU SALVAR");

    fread(&sPlayer, sizeof(Player), 1, saveFile);
    fread(sMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);
    fread(sPlantMap, sizeof(Plant) * MAP_WIDTH * MAP_HEIGHT, 1, saveFile);

    fclose(saveFile);
    printf("JOGO CARREGADO COM SUCESSO");
}