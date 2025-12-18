#include "Game.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stardew Valley do Paraguai");
    InitAudioDevice();
    SetTargetFPS(60);

    GameState game{};
    InitGame(game);
    // Loop do jogo
    while (!WindowShouldClose()) {
        UpdateGame(game);
        DrawGame(game);
    }

    // De-Initialization
    UnloadGame(game);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
