#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "bitBoard.h"
#include "move.h"
#include "checkWin.h"

#define DEPTH 3
#define HASHSIZE 10007

typedef struct {
    ULL hash;
    int score;
} TableEntry;

ULL zobristHash[WIDTH][HEIGHT][2];
TableEntry **HashMap;

int getHashSize(int depth) {
    return (depth)*HASHSIZE;
}

TableEntry** initializeHashTable() {
    TableEntry** hashTable = malloc(sizeof(TableEntry*)*DEPTH);
    if (hashTable == NULL) {
        printf("Couldn't allocate hastable\n");
        return NULL;
    }
    for (int i = 0; i < DEPTH; i++) {
        hashTable[i] = malloc(sizeof(TableEntry)*getHashSize(i));
        if (hashTable[i] == NULL) {
            printf("Error: Failed to allocate memory for hash table.\n");
            for (int j = 0; j < i; j++) {
                free(hashTable[j]);
            }
            free(hashTable);
            return NULL;
        }
        for (int j = 0; j < getHashSize(i); j++) {
            hashTable[i][j].hash = 0ULL;
            hashTable[i][j].score = 0;
        }
    }

    return hashTable;
}

int hashTableIndex(ULL hash, int depth) {
    return hash % getHashSize(depth);
}

void initializeZobristHashTable() {
    srand(time(NULL));
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int i = 0; i < 2; i++) {
                zobristHash[x][y][1] = ((unsigned long long)rand() << 32) | rand();
            }
        }
    }
}

ULL getHash(Bitboard *bitboard) {
    ULL hash = 0;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (bitboard->player1 & (1ULL << (x + y * WIDTH))) {
                hash ^= zobristHash[x][y][0];
            }
        }
    }
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (bitboard->player2 & (1ULL << (x + y * WIDTH))) {
                hash ^= zobristHash[x][y][1];
            }
        }
    }
    return hash;
}

int count = 0;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int minimax(Bitboard *board, int depth, int isMaximizingPlayer, int alpha, int beta){
    //printf("starting minimax\n");

    int aDepth = DEPTH - depth;
    ULL hash = getHash(board);
    if (aDepth > 0 && aDepth < DEPTH) {
        int size = getHashSize(aDepth);
        int index = hashTableIndex(hash, aDepth);

        if (index < size) {  
            TableEntry *entry = &HashMap[aDepth][index];

            if (entry->hash == hash) {
                printf("Hash match found: score = %d\n", entry->score);
                return entry->score;
            } else {
                printf("Hash mismatch: stored hash = %llu, current hash = %llu\n", entry->hash, hash);
            }
        } else {
            printf("Index %d is out of bounds for size %d\n", index, size);
        }
    }

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

    if (aDepth > 0 && aDepth < DEPTH) {
        int index = hashTableIndex(hash, aDepth);
        if (index < getHashSize(aDepth)) {
            TableEntry *entry = &HashMap[aDepth][index];
            entry->hash = hash;
            entry->score = bestValue;
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
    HashMap = initializeHashTable();
    initializeZobristHashTable();
    precomputeWinMasks(); 
    Bitboard game;
    boardStart(&game);


    
    //printBoard(&game);
    //printBoard(&game);


    while (1) {
        playMove(&game, &game.player1, getBestMove(&game, DEPTH, 1));

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