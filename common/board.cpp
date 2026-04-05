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
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int piece = board[y][x];
            // Check if the piece belongs to the player (normal or king)
            if (piece == player || piece == player + 2) {
                if (canCaptureAgain(x, y, player)) {
                    return true; 
                }
            }
        }
    }
    return false;
}

bool isValidMove(int toX, int toY, int player) {
    // 1. Basic Bounds and Occupancy Checks
    if (toX < 0 || toX > 7 || toY < 0 || toY > 7) return false;
    if (selectedX == -1 || selectedY == -1) return false;
    if (board[toY][toX] != 0) return false; // Destination must be empty

    int dx = toX - selectedX;
    int dy = toY - selectedY;

    // Must move diagonally
    if (abs(dx) != abs(dy)) return false;

    int piece = board[selectedY][selectedX];
    bool isKing = (piece == 3 || piece == 4);
    int enemy = (player == 1) ? 2 : 1;
    int enemyKing = enemy + 2;

    // 2. Scan the diagonal path
    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;
    
    int enemyCount = 0;
    int lastEnemyX = -1;
    int lastEnemyY = -1;

    int currX = selectedX + stepX;
    int currY = selectedY + stepY;

    while (currX != toX && currY != toY) {
        int cell = board[currY][currX];
        
        if (cell != 0) {
            // If it's our own piece, the path is blocked
            if (cell == player || cell == player + 2) return false;
            
            // If it's an enemy, count it
            if (cell == enemy || cell == enemyKing) {
                enemyCount++;
                lastEnemyX = currX;
                lastEnemyY = currY;
            }
        }
        
        currX += stepX;
        currY += stepY;
    }

    // 3. Validation Logic
    bool boardHasCapture = hasAnyCapture(player);

    // --- CASE A: JUMPING/CAPTURING ---
    if (enemyCount == 1) {
        // Normal pieces can only jump to the square immediately behind the enemy
        if (!isKing) {
            if (abs(dx) != 2) return false; 
        }
        // If it's a king, it can land anywhere on the diagonal after the enemy
        return true; 
    }

    // --- CASE B: NORMAL MOVE (No pieces jumped) ---
    if (enemyCount == 0) {
        // If a capture is available elsewhere, this move is illegal
        if (boardHasCapture) return false;

        if (isKing) {
            return true; // Flying kings can move any distance on clear diagonal
        } else {
            // Normal pieces move 1 step forward only
            if (player == 1 && dy == 1 && abs(dx) == 1) return true;
            if (player == 2 && dy == -1 && abs(dx) == 1) return true;
        }
    }

    return false; // Path had > 1 enemy or was otherwise invalid
}

bool canCaptureAgain(int x, int y, int player)
{
    int piece = board[y][x];
    bool isKing = (piece == 3 || piece == 4);

    int enemy = (player == 1) ? 2 : 1;
    int enemyKing = enemy + 2;

    int dirs[4][2] = {
        {1,1},{-1,1},{1,-1},{-1,-1}
    };

    for (auto& d : dirs)
    {
        int midX = x + d[0];
        int midY = y + d[1];
        int landX = x + 2*d[0];
        int landY = y + 2*d[1];

        // --- обычная шашка ---
        if (!isKing)
        {
            if (landX >= 0 && landX < 8 && landY >= 0 && landY < 8)
            {
                if ((board[midY][midX] == enemy || board[midY][midX] == enemyKing)
                    && board[landY][landX] == 0)
                {
                    return true;
                }
            }
        }
        else
        {
            // --- дамка (flying king) ---
            int cx = x + d[0];
            int cy = y + d[1];
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

                cx += d[0];
                cy += d[1];
            }
        }
    }

    return false;
}

