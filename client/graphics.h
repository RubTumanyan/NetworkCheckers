#pragma once
#include <SFML/Graphics.hpp>
#include "../common/move.h"

void applyMove(const Move& move , int currentPlayer);
Move handleScreenClick(int mouseX , int mouseY , int tileSize);

void drawBoard(sf::RenderWindow& window, int tileSize);
void drawPieces(sf::RenderWindow& window, int tileSize);
