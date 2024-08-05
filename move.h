#ifndef MOVE_H
#define MOVE_H
#include "bitBoard.h"

void playMove(Bitboard *board ,ULL *side, int col);
int openColumn(Bitboard* board, int col);
void undoMove(ULL* side, int col);

#endif 