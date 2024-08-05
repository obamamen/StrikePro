#include "bitBoard.h"
#include "checkWin.h"

ULL horWinMask[WIDTH][HEIGHT];
ULL verWinMask[WIDTH][HEIGHT];
ULL diagWinMask[WIDTH][HEIGHT][2];


int checkWin(ULL *side) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x <= WIDTH - 4; x++) {

            if ((*side & horWinMask[x][y]) == horWinMask[x][y]) {
                return 1;
            }
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y <= HEIGHT - 4; y++) {

            if ((*side & verWinMask[x][y]) == verWinMask[x][y]) {
                return 1;
            }
        }
    }
    for (int x = 0; x <= WIDTH - 4; x++) {
        for (int y = 0; y <= HEIGHT - 4; y++) {

            if ((*side & diagWinMask[x][y][0]) == diagWinMask[x][y][0]) {
                return 1;
            }
        }
    }
    for (int x = 0; x <= WIDTH - 4; x++) {
        for (int y = HEIGHT-1; y > 3; y--) {

            if ((*side & diagWinMask[x][y][1]) == diagWinMask[x][y][1]) {
                return 1;
            }
        }
    }
    return 0;
}

void precomputeWinMasks() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x <= WIDTH - 4; x++) {
            ULL mask = 0;
            for (int i = 0; i < 4; i++) {
                mask |= 1ULL << (x + y * WIDTH + i);
            }
            horWinMask[x][y] = mask;
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y <= HEIGHT - 4; y++) {
            ULL mask = 0;
            for (int i = 0; i < 4; i++) {
                mask |= 1ULL << (x + (y + i) * WIDTH);
            }
            verWinMask[x][y] = mask;
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) { 
            ULL mask1 = 1ULL << (x + y * WIDTH); 
            ULL mask2 = 1ULL << (x + y * WIDTH); 
            for (int i = 0; i < 3; i++) {
                mask1 |= mask1 << (WIDTH + 1);
                mask2 |= mask2 >> (WIDTH - 1);
            }
            diagWinMask[x][y][0] = mask1;
            diagWinMask[x][y][1] = mask2;
        }
    }
}