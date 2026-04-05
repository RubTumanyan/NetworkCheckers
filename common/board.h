#pragma once
#include "move.h"

extern int board[8][8];
extern int turn;
extern int myPlayer;
extern int selectedX;
extern int selectedY;


bool hasAnyCapture(int player);
bool canCaptureAgain(int x, int y, int player);
bool isValidMove(int mouseX, int mouseY, int player);

