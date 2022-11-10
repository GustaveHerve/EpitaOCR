#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "tools.h"

#define SIZE 9


void Output(long arr[SIZE][SIZE])
{

	FILE * fp;
	fp = fopen ("grid.result", "w+");

     for (int i = 0; i < SIZE; i++)
     {
         for (int j = 0; j < SIZE; j++)
             fprintf(fp, "%ld %s",arr[i][j], " ");
         fprintf(fp, "%s","\n");
     }
     fclose(fp);
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

		        if (row == SIZE)
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

           if (solve(grid, 0, 0))
            { // if the sudoku has been solved
                 Output(grid);
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


