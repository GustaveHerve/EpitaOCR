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
    
    //double rot_mat[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};
    
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            Uint32 old_pixel = get_pixel(img, j, i);
            double y0 = height / 2;
            double x0 = width / 2;

            int x2 = cos(angle) * ((double)j - x0) - sin(angle) * (i - y0) + x0;
            int y2 = sin(angle) * ((double)j - x0) + cos(angle) * (i - y0) + y0;
            replace_pixel(new_img, x2, y2, old_pixel);

            //double old_pixcos[] = {j, i};
            //double new_pixcos[] = {0, 0}; 
            //mul(rot_mat, old_pixcos, 2, 2, 1,  new_pixcos);
            /*if(new_pixcos[0] < img->w && new_pixcos[1] < img->h)
            {*/
                //Uint8 r, g, b;
                //SDL_GetRGB(old_pixel, img->format, &r, &g, &b);
                //Uint32 newpixel = SDL_MapRGB(img->format, r, g, b);
                //replace_pixel(new_img, i, j, newpixel);
            //}
        }
    }
    //foreach pixel SDL_MapRGB then SDL_putpixel
    IMG_SavePNG(new_img, "image_rotated.png");
}
