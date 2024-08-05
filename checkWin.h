#ifndef CHECKWIN_H
#define CHECKWIN_H
#include "bitBoard.h"

extern  ULL horWinMask[WIDTH][HEIGHT];
extern  ULL verWinMask[WIDTH][HEIGHT];
extern  ULL diagWinMask[WIDTH][HEIGHT][2];

void precomputeWinMasks();
int checkWin(ULL *side);

#endif