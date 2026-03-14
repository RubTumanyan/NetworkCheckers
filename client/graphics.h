#pragma once
#include <SFML/Graphics.hpp>
#include "../common/move.h"

Move handleClick(int mouseX, int mouseY);
void applyMove(const Move& m);

void drawBoard(sf::RenderWindow& window, int tileSize);
void drawPieces(sf::RenderWindow& window, int tileSize);
