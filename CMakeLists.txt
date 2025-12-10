cmake_minimum_required(VERSION 3.11)

# Nome do seu projeto
project(MeuJogoRaylib)

# 1. Configurações do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Baixar e configurar a Raylib (FetchContent)
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0  # Você pode mudar para 'master' se quiser a versão mais recente
)

# Verifica se a Raylib já foi populada, se não, faz o download
FetchContent_MakeAvailable(raylib)

# 3. Definir o executável
add_executable(${PROJECT_NAME} "src/main.cpp"   "src/Globals.h" "src/Player.h")

# 4. Linkar a Raylib ao seu executável
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# (Opcional) Copiar assets para a pasta de build, se você tiver uma pasta 'resources'
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})