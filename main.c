#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4
#define MAX_RANDOM 3
#define MAX_HISTORY 500

int grid[GRID_SIZE][GRID_SIZE] = {0};
int buffer[GRID_SIZE];
int len = 0, temp = 0, stepCount = 0;
int score = 0, highScore = 0;

int digitLength(int n);

void displayGrid()
{
    int i, j, k, digitLen;
    printf("2048\n");
    printf("SCORE: %d\n", score);
    highScore = (score > highScore) ? score : highScore;
    printf("HIGH SCORE: %d\n", highScore);
    printf("---------------------------------\n");

    for (i = 0; i < GRID_SIZE; i++)
    {
        for (j = 0; j < GRID_SIZE; j++)
        {
            if (j == 0)
                printf("|");
            if (grid[i][j] != 0)
            {
                digitLen = digitLength(grid[i][j]);
                for (k = 0; k < 4 - len; k++)
                    printf(" ");
                printf("%d", grid[i][j]);
                for (k = 0; k < 4 - len + (len - 1); k++)
                    printf(" ");
                printf("|");
            }
            else
            {
                for (k = 0; k < 7; k++)
                    printf(" ");
                printf("|");
            }
            len = 0;
        }
        if (i != GRID_SIZE - 1)
            printf("\n---------------------------------\n");
    }
    printf("\n---------------------------------\n");
    printf("[P] Undo  [R] Restart  [U] Exit  [WASD] Move\n");
    printf("Input: ");
}

void slideValues(int start)
{
    int i;
    if (start == GRID_SIZE - 1)
        return;
    for (i = start; i < GRID_SIZE; i++)
    {
        if (buffer[i] != 0)
        {
            slideValues(i + 1);
            for (; i < GRID_SIZE; i++)
            {
                if (buffer[i + 1] != 0)
                    break;
                buffer[i + 1] = buffer[i];
                buffer[i] = 0;
            }
        }
    }
}

int digitLength(int n)
{
    if (n == 0)
        return len;
    len++;
    return digitLength(n / 10);
}

void insertRandomTile()
{
    int value, i, j;
    srand(time(NULL));
    do
    {
        i = rand() % (MAX_RANDOM + 1);
        j = rand() % (MAX_RANDOM + 1);
    } while (grid[i][j] != 0);

    value = 2 * ((rand() % 10) + 1);
    grid[i][j] = (value == 2 || value == 3) ? 4 : 2;
}

void combineRight()
{
    for (int i = GRID_SIZE - 1; i > 0; i--)
    {
        if (buffer[i] == buffer[i - 1])
        {
            buffer[i] += buffer[i - 1];
            score += buffer[i];
            buffer[i - 1] = 0;
            temp = 1;
        }
        else if (buffer[i - 1] == 0 && buffer[i] != 0)
        {
            buffer[i - 1] = buffer[i];
            buffer[i] = 0;
            temp = 1;
        }
        else if (buffer[i] == 0)
        {
            temp = 1;
        }
    }
    slideValues(0);
}

void saveState(int ***history)
{
    FILE *file = fopen("hstr.txt", "a");
    fprintf(file, "%d ", score);
    fclose(file);

    if (stepCount == MAX_HISTORY + 1)
    {
        for (int i = MAX_HISTORY; i > 0; i--)
            for (int j = 0; j < GRID_SIZE; j++)
                for (int k = 0; k < GRID_SIZE; k++)
                    history[i][j][k] = history[i - 1][j][k];
        stepCount = MAX_HISTORY;
    }

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            history[MAX_HISTORY - stepCount][i][j] = grid[i][j];
}

void undoMove(int ***history)
{
    if (stepCount == 0)
    {
        printf("\nNo more undo available.\n");
        return;
    }
    FILE *file = fopen("hstr.txt", "r+");
    int prevScore;
    for (int i = 0; i < stepCount; i++)
        fscanf(file, "%d ", &prevScore);
    fclose(file);
    score = prevScore;

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = history[MAX_HISTORY - stepCount][i][j];

    stepCount--;
}

void restartGame()
{
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            grid[i][j] = 0;

    system("clear || cls");
    score = 0;
    insertRandomTile();
    displayGrid();
}

int main()
{
    int ***history = malloc((MAX_HISTORY + 1) * sizeof(int **));
    for (int i = 0; i <= MAX_HISTORY; i++)
    {
        history[i] = malloc(GRID_SIZE * sizeof(int *));
        for (int j = 0; j < GRID_SIZE; j++)
        {
            history[i][j] = malloc(GRID_SIZE * sizeof(int));
        }
    }

    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    file = fopen("hstr.txt", "w");
    fclose(file);

    insertRandomTile();
    saveState(history);
    displayGrid();

    char input;
    while (1)
    {
        if (score > highScore)
        {
            file = fopen("highscore.txt", "w");
            fprintf(file, "%d", score);
            fclose(file);
        }
        input = getchar();
        while (getchar() != '\n')
            ;

        if (input == 'D' || input == 'd')
        {
            stepCount++;
            saveState(history);
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = 0; j < GRID_SIZE; j++)
                    buffer[j] = grid[i][j];
                combineRight();
                for (int j = 0; j < GRID_SIZE; j++)
                    grid[i][j] = buffer[j];
            }
        }
        else if (input == 'A' || input == 'a')
        {
            stepCount++;
            saveState(history);
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = GRID_SIZE - 1; j >= 0; j--)
                    buffer[GRID_SIZE - 1 - j] = grid[i][j];
                combineRight();
                for (int j = 0; j < GRID_SIZE; j++)
                    grid[i][GRID_SIZE - 1 - j] = buffer[j];
            }
        }
        else if (input == 'S' || input == 's')
        {
            stepCount++;
            saveState(history);
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = 0; j < GRID_SIZE; j++)
                    buffer[j] = grid[j][i];
                combineRight();
                for (int j = 0; j < GRID_SIZE; j++)
                    grid[j][i] = buffer[j];
            }
        }
        else if (input == 'W' || input == 'w')
        {
            stepCount++;
            saveState(history);
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = GRID_SIZE - 1; j >= 0; j--)
                    buffer[GRID_SIZE - 1 - j] = grid[j][i];
                combineRight();
                for (int j = 0; j < GRID_SIZE; j++)
                    grid[GRID_SIZE - 1 - j][i] = buffer[j];
            }
        }
        else if (input == 'P' || input == 'p')
        {
            undoMove(history);
            temp = 8;
        }
        else if (input == 'R' || input == 'r')
        {
            stepCount = 0;
            restartGame();
            continue;
        }
        else if (input == 'U' || input == 'u')
        {
            exit(0);
        }

        if (temp == 1)
        {
            temp = 0;
            system("clear || cls");
            insertRandomTile();
            displayGrid();
        }
        else if (temp == 8)
        {
            temp = 0;
            displayGrid();
        }
        else
        {
            int canMove = 0;
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = GRID_SIZE - 1; j > 0; j--)
                {
                    if (grid[i][j] == grid[i][j - 1] || grid[i][j] == 0 || grid[i][j - 1] == 0)
                        canMove = 1;
                    if (grid[j][i] == grid[j - 1][i] || grid[j][i] == 0 || grid[j - 1][i] == 0)
                        canMove = 1;
                }
            }
            if (!canMove)
            {
                printf("\nGAME OVER. Play again? [Y/N]: ");
                char replay = getchar();
                while (getchar() != '\n')
                    ;
                if (replay == 'Y' || replay == 'y')
                {
                    restartGame();
                }
                else
                {
                    exit(0);
                }
            }
            else
            {
                printf("\nInvalid move.\n");
            }
        }
    }
    return 0;
}
