#include <stdio.h> 
#include <limits.h>
#include "bitBoard.h"
#include "move.h"
#include "checkWin.h"

int count = 0;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int minimax(Bitboard *board, int depth, int isMaximizingPlayer, int alpha, int beta){
    //printf("starting minimax\n");
    count ++;
    int player1Win = checkWin(&board->player1);
    int player2Win = checkWin(&board->player2);

    if (player1Win == 1) {
        return player1Win * 100 + depth;
    }
    if (player2Win == 1) {
        return -player2Win * 100 - depth;
    }

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
                    return 100 + depth;
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
                    return -100 - depth;
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

int getBestMove(Bitboard *board, int depth, int isMaximizingPlayer) {
    int bestMove = -1;
    int bestValue = INT_MIN;
    for (int col = 0; col < WIDTH; col++) {
        if (openColumn(board, col)) {
            playMove(board, &board->player1, col);
            int moveValue = minimax(board, depth, !isMaximizingPlayer, INT_MIN, INT_MAX);
            printf("score %d\n", moveValue);
            undoMove(&board->player1, col);

            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = col;
            }
        }
    }

    return bestMove;
}

int main (int argc, char *argv[])
{
    precomputeWinMasks(); 
    Bitboard game;
    boardStart(&game);


    
    //printBoard(&game);
    //printBoard(&game);


    while (1) {
        playMove(&game, &game.player1, getBestMove(&game, 14, 1));

        //int player1Win = checkWin(&game->player1);
        //int player2Win = checkWin(&game->player2);

        printBoard(&game);

        int playerMove;
        printf("Enter your move (column 0-%d): ", WIDTH - 1);
        scanf("%d", &playerMove);

        while (!openColumn(&game, playerMove)) {
            printf("Column %d is full. Enter a different move: ", playerMove);
            scanf("%d", &playerMove);
        }

        playMove(&game, &game.player2, playerMove);

        printBoard(&game);
        printf("ai searched %d nodes\n\n", count);
    }
}