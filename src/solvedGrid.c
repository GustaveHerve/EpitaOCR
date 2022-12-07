#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctype.h>
#include "tools.h"
#include "image_loading.h"

void display_grid(char* unsolved, char* solved)
{
    FILE *fn; //grid unsolved 
    FILE *fs; //grid solved

    char chn;
    char chs;

    fn = fopen(unsolved, "r");
    fs = fopen(solved, "r");

    init_sdl();

    SDL_Rect dstrect = {16, 16, 150, 150};

    SDL_Surface *grid = load_image("../templates/template.png");

    if(fn && fs)
    {
        while((chn = fgetc(fn)) != EOF && (chs = fgetc(fs)) != EOF)
        {

            if(isdigit(chn))
            {
                SDL_Surface *digit;
                if(chn == '1')
                {
                    digit= load_image("../templates/black/1.png");
                }
                else if(chn == '2')
                {
                    digit= load_image("../templates/black/2.png");
                }
                else if(chn == '3')
                {
                    digit= load_image("../templates/black/3.png");
                }
                else if(chn == '4')
                {
                    digit= load_image("../templates/black/4.png");
                }
                else if(chn == '5')
                {
                    digit= load_image("../templates/black/5.png");
                }
                else if(chn == '6')
                {
                    digit= load_image("../templates/black/6.png");
                }
                else if(chn == '7')
                {
                    digit= load_image("../templates/black/7.png");
                }
                else if(chn == '8')
                {
                    digit= load_image("../templates/black/8.png");
                }
                else
                {
                    digit= load_image("../templates/black/9.png");
                }

                SDL_BlitSurface(digit, NULL, grid, &dstrect);
                SDL_FreeSurface(digit);
                if(dstrect.x >= 1360)
                {
                    dstrect.x = 16;
                    dstrect.y += 168;
                }
                else
                    dstrect.x += 168;

                /*

                char* filepath = malloc(sizeof(char)*22);
                char* path = "templates/black/";
                char* ext = ".png";
               
                strcat(filepath, path);
                strcat(filepath, &chn); 
                strcat(filepath, ext);

                SDL_Surface *digit = load_image(filepath);
                SDL_BlitSurface(digit, NULL, grid, &dstrect); 

                SDL_FreeSurface(digit);
                free(filepath);
               
                if(dstrect.x >= 1528) //1528 = 16+9*168
                {
                    dstrect.x = 16;
                    dstrect.y += 168;
                }

                */

            }
            else if(chn == '.')
            {
                SDL_Surface *digit = NULL;
                if(chs == '1')
                {    
                    digit= load_image("../templates/red/1.png");
                }
                else if(chs == '2')
                {    
                    digit= load_image("../templates/red/2.png");
                }
                else if(chs == '3')
                {    
                    digit= load_image("../templates/red/3.png");
                }
                else if(chs == '4')
                {    
                    digit= load_image("../templates/red/4.png");
                }
                else if(chs == '5')
                {    
                    digit= load_image("../templates/red/5.png");
                }
                else if(chs == '6')
                {    
                    digit= load_image("../templates/red/6.png");
                }
                else if(chs == '7')
                {    
                    digit= load_image("../templates/red/7.png");
                }
                else if(chs == '8')
                {    
                    digit= load_image("../templates/red/8.png");
                }
                else
                {    
                    digit= load_image("../templates/red/9.png");
                }

                SDL_BlitSurface(digit, NULL, grid, &dstrect);
                SDL_FreeSurface(digit);
                if(dstrect.x >= 1360)
                {
                    dstrect.x = 16;
                    dstrect.y += 168;
                }
                else
                    dstrect.x += 168;
                /*
                char* filepath = malloc(sizeof(char)*20);
                char* path = "templates/red/";
                char* ext = ".png";
               
                strcat(filepath, path);
                strcat(filepath, &chs); 
                strcat(filepath, ext);

                SDL_Surface *digit = load_image(filepath);
                SDL_BlitSurface(digit, NULL, grid, &dstrect); 

                SDL_FreeSurface(digit);
                free(filepath);
                
                if(dstrect.x >= 1528)
                {
                    dstrect.x = 16;
                    dstrect.y += 168;
                }
                */
            }
        }

    }
    
    IMG_SavePNG(grid, "grid_solved.png");
    SDL_FreeSurface(grid);
    IMG_Quit();
    SDL_Quit();
    fclose(fn);
    fclose(fs);
}

/*
int main(int argc, char** argv)
{
    if (argc != 3)
        printf("Too few arguments");
    display_grid(argv[1], argv[2]);
}
*/
