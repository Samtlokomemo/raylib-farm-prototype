# 🌾 Project Harvest (C++ Farm Prototype)

![C++](https://img.shields.io/badge/Language-C++17-blue)
![Engine](https://img.shields.io/badge/Engine-Raylib-orange)
![Build](https://img.shields.io/badge/Build-CMake-green)

Um protótipo de jogo de simulação de fazenda desenvolvido em C++ moderno e Raylib. O objetivo deste projeto não é apenas criar um jogo, mas explorar arquitetura de game engines, gerenciamento de memória e separação de responsabilidades sem o uso de engines comerciais pesadas.

![imagem](https://i.imgur.com/1krEzEq.gif)

## 🎯 Destaques Técnicos

Ao contrário de usar Unity ou Godot, este projeto foca na implementação de sistemas baixo nível:

* **Arquitetura Desacoplada:** Lógica de jogo separada da camada de renderização.
* **Sistema de Build Modular:** Uso de **CMake** para gerenciamento de dependências e compilação cross-platform.
* **Game Loop Customizado:** Controle manual de delta-time e atualizações de física.
* **Gerenciamento de Recursos:** Carregamento e descarregamento de texturas/sons utilizando RAII para evitar memory leaks.

## 🕹️ Funcionalidades Atuais

- [x] Movimentação do personagem com colisão básica.
- [x] Sistema de Tiles (Renderização do mapa).
- [ ] Sistema de Inventário (WIP).
- [ ] Mecânica de Plantio e Colheita.
