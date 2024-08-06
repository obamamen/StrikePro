#include <stdio.h> 
#include <limits.h>
#include "bitBoard.h"
#include "move.h"
#include "checkWin.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int minimax(Bitboard *board, int depth, int isMaximizingPlayer, int alpha, int beta){
    printf("starting minimax\n");

    int player1Win = checkWin(&board->player1);
    int player2Win = checkWin(&board->player2);

    if (depth == 0) {
        return 0;
    }

    int bestValue = (isMaximizingPlayer == 1)? INT_MIN : INT_MAX;
    if (isMaximizingPlayer) {
        for (int col = 0; col < WIDTH; col++) {
            if (openColumn(board, col)) {
                playMove(board, &board->player1, col);
                int alreadyWon = checkWin(&board->player1);
                undoMove(&board->player1, col);
                alpha = max(alpha, bestValue);
                if (alreadyWon) {
                    return 1;
                }
            }
        }
        for (int col = 0; col < WIDTH; col++) {
            if (openColumn(board, col)) {
                playMove(board, &board->player1, col);
                bestValue = max(bestValue, minimax(board, depth - 1, 0, alpha, beta));
                undoMove(&board->player1, col);
                alpha = max(alpha, bestValue);
                if (beta <= alpha) break;
            }
        }
    } else {
        for (int col = 0; col < WIDTH; col++) {
            if (openColumn(board, col)) {
                playMove(board, &board->player2, col);
                int alreadyWon = checkWin(&board->player2);
                undoMove(&board->player2, col);
                beta = min(beta, bestValue);
                if (alreadyWon) {
                    return -1;
                }
            }
        }
        for (int col = 0; col < WIDTH; col++) {
            if (openColumn(board, col)) {
                playMove(board, &board->player2, col);
                bestValue = min(bestValue, minimax(board, depth - 1, 1, alpha, beta));
                undoMove(&board->player2, col);
                beta = min(beta, bestValue);
                if (beta <= alpha) break;
            }
        }
    }

    return bestValue;
}

int main (int argc, char *argv[])
{
    precomputeWinMasks(); 
    Bitboard game;
    boardStart(&game);
    playMove(&game, &game.player2, 3);
    playMove(&game, &game.player2, 2);
    playMove(&game, &game.player2, 4);
    



    printf("MiniMax: %d\n", minimax(&game, 3, 1, INT_MIN, INT_MAX));
    printBoard(&game);


    while (1) {
        // or window will close
    }
}