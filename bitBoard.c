#include <stdio.h> 
#include "bitBoard.h"

void boardStart(Bitboard* board) {
    board->player1 = 0;
    board->player2 = 0;
}

void printBoard(Bitboard* board) {
    printf("\n|");
    for (int t = 0; t < WIDTH; t++) { 
        printf("%d|", t);
    }
    printf("\n-");
    for (int t = 0; t < WIDTH; t++) { 
        printf("--");
    }
    printf("\n");
    for (int y = 0; y < HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < WIDTH; x++) {
            unsigned long long mask = 1ULL << (x + y * (WIDTH));
            if (board->player1 & mask) {
                printf("X|");
            } else if (board->player2 & mask) {
                printf("O|");
            } else {
                printf(" |");
            }
        }
        printf("\n");
    }
    printf("\n");
}