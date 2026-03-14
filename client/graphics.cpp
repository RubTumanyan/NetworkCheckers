#include "graphics.h"
#include "../common/board.h"
#include "../common/move.h"
#include <iostream>


Move handleClick(int mouseX, int mouseY)
{
    static int selectedX = -1;
    static int selectedY = -1;

    Move result{-1, -1, -1, -1}; // default = no move

    if(selectedX == -1 && selectedY == -1) {
        if(board[mouseY][mouseX] != 0) {
            selectedX = mouseX;
            selectedY = mouseY;
        }
    } else {
        int dx = mouseX - selectedX;
        int dy = mouseY - selectedY;

        // normal move (no jump)
        if(abs(dx) == 1 && ((board[selectedY][selectedX] == 1 && dy == 1) ||
                            (board[selectedY][selectedX] == 2 && dy == -1))) 
        {
            result.fromX = selectedX;
            result.fromY = selectedY;
            result.toX = mouseX;
            result.toY = mouseY;

            // update board locally
            board[mouseY][mouseX] = board[selectedY][selectedX];
            board[selectedY][selectedX] = 0;
        }

        // jump (capture)
        else if(abs(dx) == 2 && abs(dy) == 2) {
            int midX = (selectedX + mouseX)/2;
            int midY = (selectedY + mouseY)/2;

            // simple check: jump opponent
            if(board[selectedY][selectedX] == 1 && board[midY][midX] == 2) {
                result = {selectedX, selectedY, mouseX, mouseY};
                board[mouseY][mouseX] = board[selectedY][selectedX];
                board[selectedY][selectedX] = 0;
                board[midY][midX] = 0;
            }
            else if(board[selectedY][selectedX] == 2 && board[midY][midX] == 1) {
                result = {selectedX, selectedY, mouseX, mouseY};
                board[mouseY][mouseX] = board[selectedY][selectedX];
                board[selectedY][selectedX] = 0;
                board[midY][midX] = 0;
            }
        }

        selectedX = -1;
        selectedY = -1;
    }

    return result;
}

// --- applyMove: update board with a received move ---
void applyMove(const Move& m)
{
    if(m.fromX == -1) return; // invalid move
    board[m.toY][m.toX] = board[m.fromY][m.fromX];
    board[m.fromY][m.fromX] = 0;

    // handle capture if jumping
    if(abs(m.toX - m.fromX) == 2) {
        int midX = (m.fromX + m.toX)/2;
        int midY = (m.fromY + m.toY)/2;
        board[midY][midX] = 0;
    }
}

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
