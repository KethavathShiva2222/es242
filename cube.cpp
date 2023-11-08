#include <cstdio>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <queue>
#include <math.h>
#include <assert.h>
#include <utility>
using namespace std;

struct board {
    int e[7];
};
struct colab {
    char c[6][4];
};
board transistion(const colab& b){
    board ans;
    string str;
    string arr[21]={"rgw","wrg","gwr","rwb","brw","wbr","ryg","gry","ygr","rby","yrb","byr","owg","gow","wgo","obw","wob","bwo","oyb","boy","ybo"};
    str.push_back(b.c[0][0]);
    str.push_back(b.c[3][1]);
    str.push_back(b.c[4][2]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(0)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[0][1]);
    str.push_back(b.c[4][3]);
    str.push_back(b.c[1][0]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(1)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[0][2]);
    str.push_back(b.c[5][0]);
    str.push_back(b.c[3][3]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(2)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[0][3]);
    str.push_back(b.c[1][2]);
    str.push_back(b.c[5][1]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(3)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[2][0]);
    str.push_back(b.c[4][0]);
    str.push_back(b.c[3][0]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(4)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[2][1]);
    str.push_back(b.c[1][1]);
    str.push_back(b.c[4][1]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(5)*3+(i%3);
        }
    }
    str.clear();
    str.push_back(b.c[2][3]);
    str.push_back(b.c[5][3]);
    str.push_back(b.c[1][3]);
    for (int i=0;i<21;i++){
        if (str==arr[i]){
            ans.e[i/3]=(6)*3+(i%3);
        }
    }
    return ans;
}
void print_board(const board& b)
{
    for (int r = 0; r < 7; ++r) {
        printf("%3d", b.e[r]);
    }
    printf("\n");
}

void read_board(colab& b)
{
    for (int r = 0; r < 6; ++r) {
        for(int cl = 0; cl < 4; ++cl){
            scanf("%s", &b.c[r][cl]);
        }
    }
}

board front(const board& b)
{
    board o = b;
    int arr[21]={3,4,5,9,10,11,0,1,2,6,7,8,12,13,14,15,16,17,18,19,20};
    for (int i=0;i<7;i++){
        o.e[i]=arr[o.e[i]];
    }
    return o;
}

board right(const board& b)
{
    board o = b;
    int arr[21]={0,1,2,17,15,16,6,7,8,4,5,3,12,13,14,19,20,18,11,9,10};
    for (int i=0;i<7;i++){
        o.e[i]=arr[o.e[i]];
    }
    return o;
}

board up(const board& b)
{
    board o = b;
    int arr[21]={14,12,13,1,2,0,6,7,8,9,10,11,16,17,15,5,3,4,18,19,20};
    for (int i=0;i<7;i++){
        o.e[i]=arr[o.e[i]];
    }
    return o;
}

enum move { R=1,U=2,F=3,R2=4,U2=5,F2=6,R1=7,U1=8,F1=9};

/*
 * Return a shortest path from src to dest.
 */

int ord(const board& board){
    int val=0;
    int k=6;
    for (int r = 0; r < 7; ++r) {
        int v=board.e[r];
        val+=v*pow(21,k);
        k-=1;
    }
    // printf("%d",val);
    return val;
}

board decode(int ord){
    board node;
    int temp=ord;
    
    for (int r = 6; r >=0; r--) {
        node.e[r]=temp%21;
        temp=temp/21;
    }
    // print_board(node);
    return node;
}
#define MAX (1800000000) 
std::vector<int> solve(const board& src, const board& dest)
{ 
    queue <int> q;
    int visited[MAX];
    int parent[MAX];
    int initial=ord(src);
    int final=ord(dest);
    q.push(ord(src));
    visited[ord(src)] = U;
    int temp=0;
    while (!q.empty()) {
        int child = q.front();
        q.pop();
        board u=decode(child);
        if (child==final) {
            /* return the moves to get to u from src. */
            std::vector<int> moves;
            while (child!=initial) {
                moves.push_back(visited[child]);
                child=parent[child];
            }
            std::reverse(moves.begin(), moves.end());
            std::vector<int> ans;
            int prev=0;
            for (auto cur:moves){
                auto tem=cur;
                if (cur==prev){
                    int flat=ans.back();
                    ans.pop_back();
                    if(flat==cur){
                        tem+=3;
                    }else{
                        tem+=6;
                    }
                }
                prev=cur;
                ans.push_back(tem);
            }
            return ans;
        }

        board a = front(u);
        board b = right(u);
        board c = up(u);

        int aord = ord(a);
        int bord = ord(b);
        int cord = ord(c);

        if (!visited[aord]) {
            visited[aord] = F;
            parent[aord] = child;
            q.push(aord);
        }
        if (!visited[bord]) {
            visited[bord] = R;
            parent[bord] = child;
            q.push( bord);
        }
        if (!visited[cord]) {
            visited[cord] = U;
            parent[cord] = child;
            q.push(cord);
        }
        temp+=1;
    }
    return std::vector<int>();
    assert(0);
}

void print_moves(const std::vector<int>& moves)
{
    for (auto m: moves) {
        switch (m) {
        case U: printf("U "); break;
        case F: printf("F "); break;
        case R: printf("R "); break;
        case U1: printf("U' "); break;
        case F1: printf("F' "); break;
        case R1: printf("R' "); break;
        case U2: printf("U2 "); break;
        case F2: printf("F2 "); break;
        case R2: printf("R2 "); break;
        }
    }
    printf("\n");
}

int main()
{
    board src, dest;
    colab src1;
    read_board(src1);
    board src2=transistion(src1);
    print_board(src2);
    for (int i=0;i<7;i++){
        src.e[i]=(i)*3;
        dest.e[i]=i*3;
    }
    dest=front(dest);

    auto moves = solve(src, dest);
    print_moves(moves);

    return 0;
}
