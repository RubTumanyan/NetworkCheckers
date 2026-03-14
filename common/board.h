#pragma once
#include "move.h"

extern int board[8][8];
extern int selectedX;
extern int selectedY;
extern int turn;

bool isValidMove(int mouseX, int mouseY);
void movePiece(int mouseX, int mouseY);
Move handleClick(int mouseX, int mouseY);
