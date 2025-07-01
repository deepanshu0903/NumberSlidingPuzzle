#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 8
#define MAX_NAME 20

int board[MAX_SIZE][MAX_SIZE];
int size;
int moveCount = 0;
char playerName[MAX_NAME];

void generatePuzzle() {
    int nums[MAX_SIZE * MAX_SIZE];
    for (int i = 0; i < size * size; i++) nums[i] = i;

    for (int i = size * size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }

    int k = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = nums[k++];
}

void displayBoard() {
    printf("\nMoves: %d\n", moveCount);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0)
                printf("   ");
            else
                printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}

void findZero(int *x, int *y) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0) {
                *x = i;
                *y = j;
                return;
            }
}

void moveTile(char dir) {
    int x, y;
    findZero(&x, &y);
    int nx = x, ny = y;

    switch (dir) {
        case 'w': nx = x - 1; break;
        case 's': nx = x + 1; break;
        case 'a': ny = y - 1; break;
        case 'd': ny = y + 1; break;
        default: printf("Invalid move!\n"); return;
    }

    if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
        int temp = board[nx][ny];
        board[nx][ny] = 0;
        board[x][y] = temp;
        moveCount++;
    } else {
        printf("Out of bounds!\n");
    }
}

int isSolved() {
    int count = 1;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (i == size - 1 && j == size - 1)
                return board[i][j] == 0;
            if (board[i][j] != count++)
                return 0;
        }
    return 1;
}

void updateLeaderboard(const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(file, "%s %d\n", playerName, moveCount);
        fclose(file);
    }
}

void viewLeaderboard(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No leaderboard data found.\n");
        return;
    }

    printf("\n--- Leaderboard (%s) ---\n", filename);
    char name[MAX_NAME];
    int score, rank = 1;
    while (fscanf(file, "%s %d", name, &score) != EOF && rank <= 5) {
        printf("%d. %s - %d moves\n", rank++, name, score);
    }
    fclose(file);
}

int main() {
    srand(time(0));
    int level;
    char leaderboardFile[20];

    printf("Enter your name: ");
    scanf("%s", playerName);

    printf("\nChoose Difficulty:\n");
    printf("1. Easy (3x3)\n");
    printf("2. Moderate (4x4)\n");
    printf("3. Hard (8x8)\n");
    printf("Enter choice: ");
    scanf("%d", &level);

    switch (level) {
        case 1: size = 3; strcpy(leaderboardFile, "easy.txt"); break;
        case 2: size = 4; strcpy(leaderboardFile, "moderate.txt"); break;
        case 3: size = 8; strcpy(leaderboardFile, "hard.txt"); break;
        default: printf("Invalid! Defaulting to Easy\n"); size = 3; strcpy(leaderboardFile, "easy.txt");
    }

    generatePuzzle();

    while (1) {
        displayBoard();
        if (isSolved()) {
            printf("\n🎉 %s solved it in %d moves!\n", playerName, moveCount);
            updateLeaderboard(leaderboardFile);
            viewLeaderboard(leaderboardFile);
            break;
        }
        printf("Move (W/A/S/D): ");
        char move;
        scanf(" %c", &move);
        moveTile(move);
    }

    return 0;
}
