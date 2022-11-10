#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define N 9

//This function creates a new file
//with the solved sudoku on it
void print(long arr[N][N])
{

	FILE * fp;
	fp = fopen ("grid.result", "w+");

     for (int i = 0; i < N; i++)
     {
         for (int j = 0; j < N; j++)
             fprintf(fp, "%d %s",arr[i][j], " ");
         fprintf(fp, "%s","\n");
     }
     fclose(fp);
}

// function used to check if it's safe
// to place a value (backtracking method)

int isSafe(long grid[N][N], long row, long col, long num)
{

    // Check if we find the same num
    // in the similar row and column, we return 0
    for (int x = 0; x < 9; x++)
        if (grid[row][x] == num || grid[x][col] == num)
            return 0;

    // Check if we find the same num in the
    // particular 3*3 matrix, we return 0
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

/* main sudoku function that solves using the backtracking method */
int solveSudoku(long grid[N][N], long row, long col)
{

    // Check if we have reached the end
    // of the matrice, return 1 to stop recursive.
    if (row == N - 1 && col == N)
        return 1;

    //  Check if column reaches the end,
    //  we move to next row and
    //  column start from 0
    if (col == N)
    {
        row++;
        col = 0;
    }

    // Check if the current position
    // of the grid already contains
    // a value (0 for empty) we iterate for next column
    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= N; num++)
    {

        // Check if it is safe to place
        // the num (1-9)  in the
        // given row ,col  ->we move to next column
        if (isSafe(grid, row, col, num))
        {
           // As it's safe we place the num in the empty spot
            grid[row][col] = num;

            //  Recursive call, it returns 1 when the sudoku is solved
            if (solveSudoku(grid, row, col + 1))
                return 1;
        }
        // if it isn't "safe" then we remove the value and place
        // and place the next one we'll get with the loop
        grid[row][col] = 0;
    }
    return 0; //if we can't place any values then the sudoku is not solvable
}



int main(int argc, char *argv[])
{
 FILE *fp;
 char *filename;
 char ch;
 long row = 0;
 long col = 0;
 long (*grid)[9] = malloc(sizeof(long[9][9]));

    if (argc < 2)
    {
        printf("To few args");
        return 1;
    }

   filename = argv[1];

   // Open file in read-only mode
   fp = fopen(filename,"r");

   // If file opened successfully we start reading the given file
   // and transform the given sudoku in multiple char format into
   // a int matrice.
   if ( fp )
      {
           while ( (ch = fgetc(fp)) != EOF ) // EOF is End Of File
           {

		        if (row == 9)
                { // if we reach end of row, new column
			        row = 0;
			        col++;
		        }

		        if (isdigit(ch))
                { // check if number (1-9)
			        grid[col][row]= ch - '0';
		            row++;
		        }

                else if(ch == '.')
                { // check if empty spot
			        grid[col][row] = 0;
			        row++;
		        }


           }

           if (solveSudoku(grid, 0, 0))
            { // if the sudoku has been solved
                 print(grid);
            } // We write it into the result file

           else
           {
                printf("Unsolvable sudoku");
                return 1;
           }
       }
   else
      {
         printf("Failed to open the file\n");
         return 1;
      }

fclose(fp);
free(grid);

return 0;
}


