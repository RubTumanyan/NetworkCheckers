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

bool hasAnyCapture(int player) {
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            if(board[y][x] == player || board[y][x] == player + 2) {
                if(canCaptureAgain(x, y, player))
                    return true;
            }
        }
    }
    return false;
}


bool isValidMove(int mouseX, int mouseY, int player)
{	    if (selectedX != -1 && selectedY != -1 && canCaptureAgain(selectedX, selectedY, player)) {
	    if (mouseX != selectedX || mouseY != selectedY) {
        return false; // must move the same piece
    }
	}
    // bounds
    if (mouseX < 0 || mouseX > 7 || mouseY < 0 || mouseY > 7)
        return false;

    if (selectedX == -1 || selectedY == -1)
        return false;

    if (board[mouseY][mouseX] != 0)
        return false;

    int dx = mouseX - selectedX;
    int dy = mouseY - selectedY;

    if (abs(dx) != abs(dy))
        return false; // must move diagonally

    int piece = board[selectedY][selectedX];
    bool isKing = (piece == 3 || piece == 4);

    int enemy = (player == 1) ? 2 : 1;
    int enemyKing = enemy + 2;

    bool mustCapture = hasAnyCapture(player);

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    int x = selectedX + stepX;
    int y = selectedY + stepY;

    int enemyCount = 0;

    // 🔥 scan path
    while (x != mouseX && y != mouseY)
    {
        if (board[y][x] != 0)
        {
            if (board[y][x] == enemy || board[y][x] == enemyKing)
            {
                enemyCount++;
                if (enemyCount > 1) return false;
            }
            else return false; // own piece blocks
        }

        x += stepX;
        y += stepY;
    }

    // 🔥 MUST CAPTURE
    if (mustCapture)
        return enemyCount == 1;

    // 🔥 KING
    if (isKing)
    {
        // normal move
        if (enemyCount == 0)
            return true;

        // capture
        return enemyCount == 1;
    }

    // 🔥 NORMAL PIECE

    // normal move (1 step forward)
    if (enemyCount == 0)
    {
        if (player == 1 && dy == 1) return true;
        if (player == 2 && dy == -1) return true;
        return false;
    }

    // capture (exactly 2 steps)
    if (enemyCount == 1 && abs(dx) == 2)
        return true;

    return false;
}
         




bool canCaptureAgain(int x, int y, int player)
{
    int piece = board[y][x];
    bool isKing = (piece == 3 || piece == 4);

    int enemy = (player == 1) ? 2 : 1;
    int enemyKing = enemy + 2;

    int directions[4][2] = {
        {1,1}, {-1,1}, {1,-1}, {-1,-1}
    };

    for (auto& d : directions)
    {
        int stepX = d[0];
        int stepY = d[1];

        int cx = x + stepX;
        int cy = y + stepY;

        bool foundEnemy = false;

        while (cx >= 0 && cx < 8 && cy >= 0 && cy < 8)
        {
            if (board[cy][cx] != 0)
            {
                if (board[cy][cx] == enemy || board[cy][cx] == enemyKing)
                {
                    if (foundEnemy) break;
                    foundEnemy = true;
                }
                else break;
            }
            else if (foundEnemy)
            {
                return true;
            }

            // 🔥 normal piece stops after 2 steps
            if (!isKing) break;

            cx += stepX;
            cy += stepY;
        }
    }

    return false;
}
