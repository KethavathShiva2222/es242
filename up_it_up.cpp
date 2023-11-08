#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <queue>
#include <cmath>
#include <cassert>
#include <utility>
using namespace std;

struct Board {
    int elements[3][3];
};

void PrintBoard(const Board& board) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            printf("%3d", board.elements[row][col]);
        }
        printf("\n");
    }
}

void ReadBoard(Board& board) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            scanf("%d", &board.elements[row][col]);
        }
    }
}

tuple<int, int> FindSpace(const Board& board) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board.elements[row][col] == 0) {
                return {row, col};
            }
        }
    }
    assert(0);
}

Board MoveUp(const Board& board) {
    const auto [row, col] = FindSpace(board);
    if (row == 0) return board;
    Board newBoard = board;
    if (newBoard.elements[row - 1][col] == 1) {
        newBoard.elements[row][col] = 6;
    } else if (newBoard.elements[row - 1][col] == 2) {
        newBoard.elements[row][col] = 5;
    } else if (newBoard.elements[row - 1][col] == 5) {
        newBoard.elements[row][col] = 1;
    } else if (newBoard.elements[row - 1][col] == 6) {
        newBoard.elements[row][col] = 2;
    } else {
        newBoard.elements[row][col] = newBoard.elements[row - 1][col];
    }
    newBoard.elements[row - 1][col] = 0;
    return newBoard;
}

Board MoveDown(const Board& board) {
    const auto [row, col] = FindSpace(board);
    if (row == 2) return board;
    Board newBoard = board;
    if (newBoard.elements[row + 1][col] == 1) {
        newBoard.elements[row][col] = 5;
    } else if (newBoard.elements[row + 1][col] == 2) {
        newBoard.elements[row][col] = 6;
    } else if (newBoard.elements[row + 1][col] == 5) {
        newBoard.elements[row][col] = 2;
    } else if (newBoard.elements[row + 1][col] == 6) {
        newBoard.elements[row][col] = 1;
    } else {
        newBoard.elements[row][col] = newBoard.elements[row + 1][col];
    }
    newBoard.elements[row + 1][col] = 0;
    return newBoard;
}

Board MoveLeft(const Board& board) {
    const auto [row, col] = FindSpace(board);
    if (col == 0) return board;
    Board newBoard = board;
    if (newBoard.elements[row][col - 1] == 1) {
        newBoard.elements[row][col] = 4;
    } else if (newBoard.elements[row][col - 1] == 4) {
        newBoard.elements[row][col] = 2;
    } else if (newBoard.elements[row][col - 1] == 2) {
        newBoard.elements[row][col] = 3;
    } else if (newBoard.elements[row][col - 1] == 3) {
        newBoard.elements[row][col] = 1;
    } else {
        newBoard.elements[row][col] = newBoard.elements[row][col - 1];
    }
    newBoard.elements[row][col - 1] = 0;
    return newBoard;
}

Board MoveRight(const Board& board) {
    const auto [row, col] = FindSpace(board);
    if (col == 2) return board;
    Board newBoard = board;
    if (newBoard.elements[row][col + 1] == 1) {
        newBoard.elements[row][col] = 3;
    } else if (newBoard.elements[row][col + 1] == 3) {
        newBoard.elements[row][col] = 2;
    } else if (newBoard.elements[row][col + 1] == 2) {
        newBoard.elements[row][col] = 4;
    } else if (newBoard.elements[row][col + 1] == 4) {
        newBoard.elements[row][col] = 1;
    } else {
        newBoard.elements[row][col] = newBoard.elements[row][col + 1];
    }
    newBoard.elements[row][col + 1] = 0;
    return newBoard;
}

enum Move { LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };

int Ord(const Board& board) {
    int value = 0;
    int k = 8;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int v = board.elements[row][col];
            value += v * pow(7, k);
            k -= 1;
        }
    }
    return value;
}

Board Decode(int order) {
    Board node;
    int temp = order;

    for (int row = 2; row >= 0; row--) {
        for (int col = 2; col >= 0; col--) {
            node.elements[row][col] = temp % 7;
            temp = temp / 7;
        }
    }
    // PrintBoard(node);
    return node;
}

#define MaxPossibleOrders (40355000)
vector<int> Solve(const Board& src, const Board& dest) {
    queue<int> q;
    int visited[MaxPossibleOrders];
    int parent[MaxPossibleOrders];
    int initialOrder = Ord(src);
    int finalOrder = Ord(dest);
    q.push(Ord(src));
    visited[Ord(src)] = LEFT;
    int temp = 0;
    while (!q.empty()) {
        int childOrder = q.front();
        q.pop();
        Board u = Decode(childOrder);
        if (childOrder == finalOrder) {
            // Return the moves to get to u from src.
            vector<int> moves;
            while (childOrder != initialOrder) {
                moves.push_back(visited[childOrder]);
                childOrder = parent[childOrder];
            }
            reverse(moves.begin(), moves.end());
            return moves;
        }

        Board a = MoveUp(u);
        Board b = MoveDown(u);
        Board c = MoveLeft(u);
        Board d = MoveRight(u);

        int aOrder = Ord(a);
        int bOrder = Ord(b);
        int cOrder = Ord(c);
        int dOrder = Ord(d);

        if (!visited[aOrder]) {
            visited[aOrder] = UP;
            parent[aOrder] = childOrder;
            q.push(aOrder);
        }
        if (!visited[bOrder]) {
            visited[bOrder] = DOWN;
            parent[bOrder] = childOrder;
            q.push(bOrder);
        }
        if (!visited[cOrder]) {
            visited[cOrder] = LEFT;
            parent[cOrder] = childOrder;
            q.push(cOrder);
        }
        if (!visited[dOrder]) {
            visited[dOrder] = RIGHT;
            parent[dOrder] = childOrder;
            q.push(dOrder);
        }
        temp += 1;
    }
    return vector<int>();
    assert(0);
}

void PrintMoves(const vector<int>& moves) {
    for (auto move : moves) {
        switch (move) {
        case LEFT: printf("L "); break;
        case RIGHT: printf("R "); break;
        case UP: printf("U "); break;
        case DOWN: printf("D "); break;
        }
    }
    printf("\n");
}

int main() {
    Board src, dest;

    // ReadBoard(src);
    // ReadBoard(dest);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            src.elements[row][col] = 1;
            dest.elements[row][col] = 2;
        }
    }
    src.elements[1][1] = 0;
    dest.elements[1][1] = 0;

    PrintBoard(src);
    PrintBoard(dest);
    auto moves = Solve(src, dest);
    PrintMoves(moves);

    return 0;
}
