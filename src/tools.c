#include <stdio.h>
#include <ctype.h>
#include "../include/tools.h"

#define SIZE 9
#define SQUARE 3

// function used to check if it's safe
// to place a value (backtracking method)

int CheckNumber(long grid[SIZE][SIZE], long row, long col, long val)
{

    // Check if we find the same num
    // in the similar row and column, we return 0
    for (int x = 0; x < SIZE; x++)
        if (grid[row][x] == val || grid[x][col] == val)
            return 0;

    // Check if we find the same num in the
    // particular 3*3 matrix, we return 0
    int startRow = row - (row % SQUARE);
    int startCol = col - (col % SQUARE);

    for (int i = 0; i < SQUARE; i++)
        for (int j = 0; j < SQUARE; j++)
            if (grid[i + startRow][j + startCol] == val)
                return 0;

    return 1;
}

/* main sudoku function that solves using the backtracking method */
int solve(long grid[SIZE][SIZE], long row, long col)
{

    // Check if we have reached the end
    // of the matrice, return 1 to stop recursive.
    if (row == SIZE - 1 && col == SIZE)
        return 1;

    //  Check if column reaches the end,
    //  we move to next row and
    //  column start from 0
    if (col == SIZE)
    {
        row++;
        col = 0;
    }

    // Check if the current position
    // of the grid already contains
    // a value (0 for empty) we iterate for next column
    if (grid[row][col] > 0)
        return solve(grid, row, col + 1);

    for (int val = 1; val <= SIZE; val++)
    {

        // Check if it is safe to place
        // the num (1-9)  in the
        // given row ,col  ->we move to next column
        if (CheckNumber(grid, row, col, val))
        {
           // As it's safe we place the num in the empty spot
            grid[row][col] = val;

            //  Recursive call, it returns 1 when the sudoku is solved
            if (solve(grid, row, col + 1))
                return 1;
        }
        // if it isn't "safe" then we remove the value and place
        // and place the next one we'll get with the loop
        grid[row][col] = 0;
    }
    return 0; //if we can't place any values then the sudoku is not solvable
}

