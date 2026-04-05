#include "graphics.h"
#include "../common/board.h"
#include "../common/move.h"
#include <iostream>
#include <cmath>

extern int currentPlayer;
extern int board[8][8];


Move handleScreenClick(int mouseX, int mouseY, int tileSize)
{   //devide 80picels
    int x = mouseX / tileSize; 
    int y = mouseY / tileSize;

    if (x < 0 || x > 7 || y < 0 || y > 7) return Move{-1,-1,-1,-1};

    // FIRST SELECTION
    if (selectedX == -1 && selectedY == -1) {
        // CHECK THE PIECIES OUR'S OR NOT
        if (board[y][x] != 0 && (board[y][x] == currentPlayer || board[y][x] == currentPlayer + 2)) {
            
            // MANDATORY RULE
            if (hasAnyCapture(currentPlayer)) {
                // Only allow selection if THIS specific piece can capture
                if (canCaptureAgain(x, y, currentPlayer)) {
                    selectedX = x;
                    selectedY = y;
                } else {
                    std::cout << "Invalid selection: You must capture with another piece!" << std::endl;
                }
            } else {
                // No captures available anywhere, free to select any of our pieces
                selectedX = x;
                selectedY = y;
            }
        }
        return Move{-1, -1, -1, -1}; 
    }

    // SECOND SELECTION
    if(!isValidMove(x, y, currentPlayer)) {
        // If we were in a multi-capture, don't reset selection to -1
        // because the player MUST finish the jump with that piece.
        if (!canCaptureAgain(selectedX, selectedY, currentPlayer)) {
            selectedX = -1;
            selectedY = -1;
        }
        return Move{-1,-1,-1,-1};
    }

    Move m{selectedX, selectedY, x, y};
    return m; 
}





void applyMove(const Move& m, int currentPlayer)
{
    // Bounds check
    if (m.fromX < 0 || m.fromX > 7 || m.fromY < 0 || m.fromY > 7 ||
        m.toX < 0 || m.toX > 7 || m.toY < 0 || m.toY > 7)
        return;

    int piece = board[m.fromY][m.fromX];
    if (piece == 0) return; 

    
    board[m.toY][m.toX] = piece;
    board[m.fromY][m.fromX] = 0;

    // Handle captures (works for flying kings too) ---
    int dx = m.toX - m.fromX;
    int dy = m.toY - m.fromY;
    


    // JUMP
    if (abs(dx) > 1 || abs(dy) > 1) 
    {
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;

        int x = m.fromX + stepX;
        int y = m.fromY + stepY;

        while (x != m.toX && y != m.toY)
        {
            if (board[y][x] != 0)
            {
                board[y][x] = 0; // remove captured piece
                break;           // only one piece per jump
            }
            x += stepX;
            y += stepY;
        }
    }

    // TRANSFORMATION TO A KING
    if (currentPlayer == 1 && m.toY == 7 && piece == 1) board[m.toY][m.toX] = 3;
    if (currentPlayer == 2 && m.toY == 0 && piece == 2) board[m.toY][m.toX] = 4;
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

            window.draw(square);//send the square to window
        }
    }
}





void drawPieces(sf::RenderWindow& window, int tileSize)
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){

            int cell = board[y][x];

            if(cell == 0)
                continue;

            sf::CircleShape piece(tileSize / 2 - 10);
            piece.setPosition(x * tileSize + 10, y * tileSize + 10);

         
            if(cell == 1)
                piece.setFillColor(sf::Color(100, 30, 30));   // player 1
            else if(cell == 2)
                piece.setFillColor(sf::Color::White);         // player 2
            else if(cell == 3) { // king player 1
                piece.setFillColor(sf::Color(200, 50, 50));
                piece.setOutlineThickness(3);
                piece.setOutlineColor(sf::Color::Yellow);
            }
            else if(cell == 4) { // king player 2
                piece.setFillColor(sf::Color(200, 200, 200));
                piece.setOutlineThickness(3);
                piece.setOutlineColor(sf::Color::Yellow);
            }

            window.draw(piece);
        }
    }
}
