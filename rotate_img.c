#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/matrix.h"

void rotate_img(SDL_Surface *img, double angle)
{
    unsigned int width = img ->w;
    unsigned int height = img->h;

    SDL_Surface *new_img = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            Uint32 old_pixel = get_pixel(img, j, i);
            double y0 = (double)height / 2;
            double x0 = (double)width / 2;

            int x2 = cos(angle) * ((double)j - x0) - sin(angle) * (i - y0) + x0;
            int y2 = sin(angle) * ((double)j - x0) + cos(angle) * (i - y0) + y0;
            replace_pixel(new_img, x2, y2, old_pixel);

        }
    }
    //foreach pixel SDL_MapRGB then SDL_putpixel
    IMG_SavePNG(new_img, "image_rotated.png");
}
