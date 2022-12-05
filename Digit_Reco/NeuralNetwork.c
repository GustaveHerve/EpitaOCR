#include "funcs.h"
#include "new_ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <string.h>

#define savefile "grid_00"

int main(int argc, char **argv)
{
    
    /* If no arguments, then we train */

    if (argc < 2)
        Call();

    /*If there is a folder path in argument then we get number of each image*/

    else 
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(argv[1]);
        FILE * fp;
        int* arr = (int*)malloc(100 * sizeof(int));
        int i = 0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (strstr(dir->d_name, "png") != NULL || strstr(dir->d_name, "jpeg") != NULL)
                {
                    char fn[100];
                    strcpy( fn, argv[1] );
                    char* s = strcat(fn,dir->d_name);
                    printf("%s \n", s);
                    arr[i] = ai(2,s,NULL);
                    i++;
                    //fprintf(fp, "%d\n", ai(2,s));
                    //Train(2,s);
                }
            }

            /* Write the result in grid_00 file */

            int tmp = 0;
            closedir(d);
            fp = fopen(savefile, "w");
            for (int i = 1; i < 10;i++)
            {
                int g = tmp;
                int h = tmp + 9;
                for (int j = g; j < h; j++)
                {
                    tmp = j;
                    if (j != 0 && j%3 == 0 && j%9 != 0)
                        fprintf(fp, "%s", " ");
                    if (arr[j] == 0)
                        fprintf(fp, "%s", ".");
                    if (arr[j] != 0)
                        fprintf(fp, "%d", arr[j]);
                }
                fprintf(fp, "%s", "\n");
                if (i != 1 && i%3 == 0 && i%9 != 0)
                    fprintf(fp, "%s", "\n");

                tmp++;
            }
            fclose(fp);
            free(arr);
        }
    }

  return 0;

}




