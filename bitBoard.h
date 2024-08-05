#ifndef BITBOARD_H
#define BITBOARD_H


typedef unsigned long long ULL;

#define WIDTH 7
#define HEIGHT 6

typedef struct {
    ULL player1;
    ULL player2;
} Bitboard;

void printBoard(Bitboard* board);
void boardStart(Bitboard* board);

#endif // BITBOARD_H