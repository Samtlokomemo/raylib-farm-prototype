#include "raylib.h"

int main() {
    // Inicialização
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib + CMake - Exemplo Básico");

    SetTargetFPS(60);

    // Loop Principal
    while (!WindowShouldClose()) {
        // Update
        // (Lógica do jogo aqui)

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Parabéns! O ambiente está configurado.", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}