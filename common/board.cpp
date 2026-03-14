#include "board.h"
#include <cmath>

int board[8][8] = {
    {0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {2,0,2,0,2,0,2,0},
    {0,2,0,2,0,2,0,2},
    {2,0,2,0,2,0,2,0}
};

int selectedX = -1;
int selectedY = -1;
int turn = 2;

bool isValidMove(int mouseX, int mouseY)
{
    int dx = mouseX - selectedX;
    int dy = mouseY - selectedY;

    // Normal move: one step diagonally
    if(turn == 1 && dy == 1 && abs(dx) == 1) return true;
    if(turn == 2 && dy == -1 && abs(dx) == 1) return true;

    // Capture move: two steps diagonally over opponent
    if(turn == 1 && dy == 2 && abs(dx) == 2){
        int midX = (selectedX + mouseX) / 2;
        int midY = (selectedY + mouseY) / 2;
        if(board[midY][midX] == 2) return true;  // opponent's piece
    }
    if(turn == 2 && dy == -2 && abs(dx) == 2){
        int midX = (selectedX + mouseX) / 2;
        int midY = (selectedY + mouseY) / 2;
        if(board[midY][midX] == 1) return true;
    }

    return false;
}

void movePiece(int mouseX, int mouseY)
{
    int dx = mouseX - selectedX;
    int dy = mouseY - selectedY;

    // Check if this is a capture
    if(abs(dx) == 2 && abs(dy) == 2){
        int midX = (selectedX + mouseX) / 2;
        int midY = (selectedY + mouseY) / 2;

        board[midY][midX] = 0; // remove captured piece
    }

    // Move piece
    board[mouseY][mouseX] = board[selectedY][selectedX];
    board[selectedY][selectedX] = 0;

    // Switch turn
    turn = (turn == 1 ? 2 : 1);

    // Reset selection
    selectedX = -1;
    selectedY = -1;
}
