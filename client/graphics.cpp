#include "graphics.h"
#include "../common/board.h"

void drawBoard(sf::RenderWindow& window, int tileSize)
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){

            sf::RectangleShape square(sf::Vector2f(tileSize,tileSize));
            square.setPosition(x * tileSize, y * tileSize);

            if((x + y) % 2 == 0)
                square.setFillColor(sf::Color::White);
            else
                square.setFillColor(sf::Color(139,69,19));

            window.draw(square);
        }
    }
}

void drawPieces(sf::RenderWindow& window, int tileSize)
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){

            if(board[y][x] == 0)
                continue;

            sf::CircleShape piece(tileSize/2 - 10);

            if(board[y][x] == 1)
                piece.setFillColor(sf::Color::Red);
            else
                piece.setFillColor(sf::Color::White);

            piece.setPosition(x * tileSize + 10, y * tileSize + 10);

            window.draw(piece);
        }
    }
}
