#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <ctype.h>

#define SIZE 9

int CheckNumber(long grid[SIZE][SIZE], long row, long col, long val);
int solve(long grid[SIZE][SIZE], long row, long col);

#endif
