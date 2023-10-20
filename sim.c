#include <stdio.h>
#include <assert.h>

/*
 * The status of a line.
 */
enum {
    NONE = 0, /* No line */
    CRIMSON = 1,
    AZURE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * 12 13 14 15 16 23 24 25 26 34 35 36 45 46 56
 * ...
 * board[14] = color of 56
 */
typedef int board_t[15];
typedef int player_t; /* A player should be CRIMSON or AZURE. */

player_t opposite_player(player_t player) {
    switch (player) {
        case CRIMSON: return AZURE;
        case AZURE: return CRIMSON;
        // default : assert(0);
    }
}

void initialize_gameboard(board_t gameboard) {
    for (int i = 0; i < 15; i++) {
        gameboard[i] = NONE;
    }
}

int has_lost(board_t gameboard, player_t player) {
    if (gameboard[0] == gameboard[1] && gameboard[0] == gameboard[5] && gameboard[0] == player) {
        return 1;
    }
    if (gameboard[0] == gameboard[2] && gameboard[0] == gameboard[6] && gameboard[0] == player) {
        return 1;
    }
    if (gameboard[0] == gameboard[3] && gameboard[0] == gameboard[7] && gameboard[0] == player) {
        return 1;
    }
    if (gameboard[0] == gameboard[4] && gameboard[0] == gameboard[8] && gameboard[0] == player) {
        return 1;
    }
    if (gameboard[1] == gameboard[2] && gameboard[1] == gameboard[9] && gameboard[1] == player) {
        return 1;
    }
    if (gameboard[1] == gameboard[3] && gameboard[1] == gameboard[10] && gameboard[1] == player) {
        return 1;
    }
    if (gameboard[1] == gameboard[4] && gameboard[1] == gameboard[11] && gameboard[1] == player) {
        return 1;
    }
    if (gameboard[2] == gameboard[3] && gameboard[2] == gameboard[12] && gameboard[2] == player) {
        return 1;
    }
    if (gameboard[2] == gameboard[4] && gameboard[2] == gameboard[13] && gameboard[2] == player) {
        return 1;
    }
    if (gameboard[3] == gameboard[4] && gameboard[3] == gameboard[14] && gameboard[3] == player) {
        return 1;
    }
    if (gameboard[5] == gameboard[6] && gameboard[5] == gameboard[9] && gameboard[5] == player) {
        return 1;
    }
    if (gameboard[5] == gameboard[7] && gameboard[5] == gameboard[10] && gameboard[5] == player) {
        return 1;
    }
    if (gameboard[5] == gameboard[8] && gameboard[5] == gameboard[11] && gameboard[5] == player) {
        return 1;
    }
    if (gameboard[6] == gameboard[7] && gameboard[6] == gameboard[12] && gameboard[6] == player) {
        return 1;
    }
    if (gameboard[6] == gameboard[8] && gameboard[6] == gameboard[13] && gameboard[6] == player) {
        return 1;
    }
    if (gameboard[7] == gameboard[8] && gameboard[7] == gameboard[14] && gameboard[7] == player) {
        return 1;
    }
    if (gameboard[9] == gameboard[10] && gameboard[9] == gameboard[12] && gameboard[9] == player) {
        return 1;
    }
    if (gameboard[9] == gameboard[11] && gameboard[9] == gameboard[13] && gameboard[9] == player) {
        return 1;
    }
    if (gameboard[10] == gameboard[11] && gameboard[10] == gameboard[14] && gameboard[10] == player) {
        return 1;
    }
    if (gameboard[12] == gameboard[13] && gameboard[12] == gameboard[14] && gameboard[12] == player) {
        return 1;
    }
    return 0;
}

int is_gameboard_full(board_t gameboard) {
    for (int i = 0; i < 15; i++) {
        if (gameboard[i] == NONE) return 0;
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
    unsigned long int moves;
} move_t;

void print_gameboard(board_t gameboard) {
    for (int i = 0; i <= 9; i++) {
        printf("%d  ", i);
    }
    for (int i = 10; i < 15; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < 15; i++) {
        switch (gameboard[i]) {
        case CRIMSON: printf("C  "); break;
        case AZURE: printf("A  "); break;
        case NONE: printf("N  ");break;
        }
    }
    printf("\n");
}

move_t optimal_move(board_t gameboard, player_t player, int counter) {
    move_t candidate;
    move_t response;
    int no_candidate = 1;

    for (int i = 0; i < 15; i++) {
        if (gameboard[i] == NONE) {
            gameboard[i] = player;
            if (is_gameboard_full(gameboard)) {
                gameboard[i] = NONE;
                return (move_t){
                    .line = i,
                    .score = -1,
                    .moves = 1,
                };
            }
            if (has_lost(gameboard, player)) {
                gameboard[i] = NONE;
                if (no_candidate) {
                    candidate = (move_t){
                        .line = i,
                        .score = -1,
                        .moves = 1,
                    };
                    no_candidate = 0;
                }
                continue;
            }
            response = optimal_move(gameboard, opposite_player(player), counter + 1);
            gameboard[i] = NONE;
            if (response.score == -1) {
                return (move_t){
                    .line = i,
                    .score = 1,
                    .moves = response.moves + 1,
                };
            }
            else {
                if (no_candidate || candidate.moves < (response.moves + 1)) {
                    candidate = (move_t){
                        .line = i,
                        .score = -1,
                        .moves = response.moves + 1,
                    };
                    if (no_candidate) {
                        no_candidate = 0;
                    }
                }
            }
        }
    }
    return candidate;
}

int main() {
    player_t player;
    board_t gameboard;
    printf("Welcome to the Game of Sim\n");
    printf("Choose your color: C for Crimson and A for Azure\n");
    char playerchar;
    scanf("%c", &playerchar);
    assert(playerchar == 'C' || playerchar == 'A');
    if (playerchar == 'C') {
        player = CRIMSON;
    }
    else {
        player = AZURE;
    }
    initialize_gameboard(gameboard);
    player_t current = CRIMSON;
    move_t response;
    int move;
    printf("Your sim board:\n");
    print_gameboard(gameboard);
    printf("Game begins, Crimson plays first!\n");

    while (1) {
        if (current == player) {
            printf("Enter your move: ");
            scanf("%d", &move);
            assert(move >= 0 && move < 15);
            assert(gameboard[move] == NONE);
            gameboard[move] = current;
            print_gameboard(gameboard);
            printf("\n");
        }
        else {
            response = optimal_move(gameboard, current, 0);
            printf("Computer's move: %d\n", response.line);
            gameboard[response.line] = current;
            print_gameboard(gameboard);
            printf("\n");
        }
        if (has_lost(gameboard, current)) {
            if (current == CRIMSON) printf("Azure won.\n");
            else printf("Crimson won.\n");
            break;
        }
        if (is_gameboard_full(gameboard)) {
            print_gameboard(gameboard);
            printf("DRAW. Bug exists, this should not happen.\n");
            break;
        }
        current = opposite_player(current);
    }
    return 0;
}
