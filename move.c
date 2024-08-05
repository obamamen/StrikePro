#include "bitBoard.h"
#include "move.h"

void playMove(Bitboard *board ,ULL *side, int col) {
    for (int y = 0; y < HEIGHT; y++) {
        ULL mask = 1ULL << (col + (HEIGHT - y - 1) * (WIDTH));
        if (!(board->player1 & mask) && !(board->player2 & mask)) {
            *side |= mask;
            break;
        }
    }
}

int openColumn(Bitboard* board, int col) {
    unsigned long long mask = 1ULL << (col);
    return !(board->player1 & mask) && !(board->player2 & mask);
} 

void undoMove(ULL* side, int col) {
    for (int y = 0; y < HEIGHT; y++) {
        ULL mask = 1ULL << (col + y * WIDTH);
        if (*side & mask) {
            *side &= ~mask;
            break;
        }
    }
}