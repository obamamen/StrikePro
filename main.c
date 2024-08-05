#include <stdio.h> 
#include "bitBoard.h"
#include "move.h"
#include "checkWin.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int main (int argc, char *argv[])
{
    precomputeWinMasks(); 
    Bitboard game;
    boardStart(&game);
    playMove(&game, &game.player1, 6);
    playMove(&game, &game.player1, 5);
    playMove(&game, &game.player1, 4);
    playMove(&game, &game.player1, 3);



    printf("Check win: %d\n", checkWin(&game.player1));
    printBoard(&game);


    while (1) {
        // or window will close
    }
}