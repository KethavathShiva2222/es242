/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define Rows (4)
#define Columns (5)
#define MaxOrders (28629151)
#define Connect (4)
#define ColumnCombinations (31)

typedef char Player;
typedef char GameBoard[Rows][Columns];

uint16_t calculatedMoves[30 * (31 * 31 * 31 * 31 * 31 - 1) + 1];

enum {
    Empty = 0,
    Red,
    Blue,
};

void InitializeBoard(GameBoard board){
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Columns; col++)
        {
            board[row][col] = '.';
        }
    }
}

void DisplayBoard(GameBoard board){
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Columns; col++)
        {
            printf("%3c ", board[row][col]);
        }
        printf("\n");
    }    
}

typedef struct {
    int col;
    int score;
} Move;

uint16_t EncodeMove(Move move){
    uint16_t b = 0;

    b |= move.col;

    switch (move.score)
    {
    case -1:
        b |= 1 << 3;
        break;
    case 0:
        b |= 1 << 4;
        break;
    case 1:
        b |= 1 << 5;
        break;
    default:
        break;
    }

    return b;
}

Move DecodeMove(uint16_t b){
    Move move;
    move.col = ((b) & 0x07);
    if (b & 0x8) move.score = -1;
    else if (b & 0x10) move.score = 0;
    else move.score = 1;

    return move;
}

int HasWon(GameBoard board, Player player){
    for (int row = 0; row < Rows; row++)
    {
        int temp = 0;
        for (int col = 0; col < Columns; col++)
        {
            if (board[row][col] == player) { temp += 1; }
            else { temp = 0; }
            if (temp == Connect) { return 1; }
        }
    }
    for (int col = 0; col < Columns; col++)
    {
        int temp = 0;
        for (int row = 0; row < Rows; row++)
        {
            if (board[row][col] == player) { temp += 1; }
            else { temp = 0; }
            if (temp == Connect) { return 1; }
        }
    }
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == board[3][3] && board[0][0] != '.'){
        return 1;
    }
    if (board[0][1] == board[1][2] && board[0][1] == board[2][3] && board[0][1] == board[3][4] && board[0][1] != '.'){
        return 1;
    }
    if (board[0][3] == board[1][2] && board[0][3] == board[2][1] && board[0][3] == board[3][0] && board[0][3] != '.'){
        return 1;
    }
    if (board[0][4] == board[1][3] && board[0][4] == board[2][2] && board[0][4] == board[3][1] && board[0][4] != '.'){
        return 1;
    }
    return 0;
}

int IsFull(GameBoard board){
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Columns; col++)
        {
            if (board[row][col] == '.') return 0;
        }
    }
    return 1;
}

Player OtherPlayer(Player player){
    switch (player)
    {
    case 'x':
        return 'o';
        break;
    case 'o':
        return 'x';
        break;
    }
}

int MyOrder(GameBoard board){
    char *position[] = { 
        "0000",
        "0001","0002",
        "0011","0012","0021","0022",
        "0111","0112","0121","0122","0211","0212","0221","0222",
        "1111","1112","1121","1122","1211","1212","1221","1222","2111","2112","2121","2122","2211","2212","2221","2222",
    };
    int ans = 0;
    char str[5] = "aaaa"; 
    int mul = 1;
    int flag = 1;
    for (int i = 0; i < 5; i++){
        flag = 1;
        for (int j = 0; j < 4; j++){
            if (board[j][i] == '.') str[j] = '0';
            else if (board[j][i] == 'x') str[j] = '1';
            else str[j] = '2';
        }
        for (int j = 0; j < 31; j++){
            if (strcmp(str, position[j]) == 0){
                ans = ans + j * mul;
                mul = mul * 31;
                flag = 0;
                break;
            }
        }
        assert(flag == 0);
    }
    return ans;
}

Move BestMove(GameBoard board, Player player){
    Move response;
    Move candidate;
    int noCandidate = 1;

    int track[5] = {0, 0, 0, 0, 0};

    for (int col = 0; col < Columns; col++)
    {
        for (int row = 0; row < Rows; row++)
        {
            if (board[row][col] != '.') { track[col]++; }
        }
    }

    int o = MyOrder(board);

    if (calculatedMoves[o]){
        return DecodeMove(calculatedMoves[o]);
    }

    for (int col = 0; col < Columns; col++)
    {
        if (track[col] != Rows){
            board[Rows - track[col] - 1][col] = player;

            if (HasWon(board, player)){
                board[Rows - track[col] - 1][col] = '.';
                candidate = (Move){
                    .col = col,
                    .score = 1
                };
                calculatedMoves[o] = EncodeMove(candidate);
                return candidate;
            }

            board[Rows - track[col] - 1][col] = '.';
        }
    }

    for (int col = 0; col < Columns; col++)
    {
        if (track[col] != Rows){
            board[Rows - track[col] - 1][col] = player;

            if (IsFull(board)){
                board[Rows - track[col] - 1][col] = '.';
                candidate = (Move){
                    .col = col,
                    .score = 0
                };
                calculatedMoves[o] = EncodeMove(candidate);
                return candidate;
            }

            response = BestMove(board, OtherPlayer(player));

            board[Rows - track[col] - 1][col] = '.';

            if (response.score == -1) {
                calculatedMoves[o] = EncodeMove(candidate = (Move){
                    .col = col,
                    .score = 1
                });
                return candidate;
            } else if (response.score == 0) {
                candidate = (Move){
                    .col = col,
                    .score = 0
                };
                noCandidate = 0;
            } else {
                if (noCandidate) {
                    candidate = (Move){
                        .col = col,
                        .score = -1
                    };
                    noCandidate = 0;
                }
            }
        }
    }

    calculatedMoves[o] = EncodeMove(candidate);
    return candidate;
}

void PrintKey()
{
    int ii = 0;
    for (int col = 0; col < Columns; ++col) {
        printf("%3d ", ii++);
    }
    printf("\n");
}

int main(){

    for (int i = 0; i < (30 * (31 * 31 * 31 * 31 * 31 - 1) + 1); i++){
        calculatedMoves[i] = 0;
    }

    char playerChar;
    printf("Welcome to the Connect 4 game!\n");
    printf("Player 'x' plays first!\n");
    printf("To choose your peg, enter 'x' or 'o':\n");
    scanf("%c", &playerChar);
    assert(playerChar == 'x' || playerChar == 'o');
    Player player = (playerChar == 'x') ? 'x' : 'o';

    Player current = 'x';
    GameBoard board;
    InitializeBoard(board);

    while(1) {
        int track[5] = {0, 0, 0, 0, 0};
        for (int col = 0; col < Columns; col++)
        {
            for (int row = 0; row < Rows; row++)
            {
                if (board[row][col] != '.') track[col]++;
            }
        }

        DisplayBoard(board);

        if (current == player) {
            PrintKey();
            int col;
            printf("Enter your move: ");
            scanf("%d", &col);
            board[Rows - track[col] - 1][col] = current;
        } else {
            Move response = BestMove(board, current);
            printf("%d\n", response.score);
            printf("Computer's move: %d\n", response.col);
            board[Rows - track[response.col] - 1][response.col] = current;
        }

        if (HasWon(board, current)) {
            DisplayBoard(board);
            printf("Player %c has won!\n", current);
            break;
        } else if (IsFull(board)) {
            DisplayBoard(board);
            printf("Draw.\n");
            break;
        }

        current = OtherPlayer(current);
    }

    return 0;
}
