#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "pixel.h"

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s = \n", s);
    for(int i = 0; i<(int)cols*(int)rows; i++)
    {
        if(i != 0 && i%cols == 0)
            printf("\n %4g", m[i]);
        else
            printf("   %4g", m[i]);
    }
    printf("\n");
}

void  mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[])
{
    for(int i = 0; i<(int)r1; i++)
    {
        for(int j = 0; j<(int) c2; j++)
        {
            for(int k = 0; k<(int)c1; k++)
            {
                r[i*c2+j] += m1[i*c1+k]*m2[k*r1+j];
            }
        }
    }
}

void rotate_img(SDL_Surface *img, double angle)
{
    unsigned int width = img ->w;
    unsigned int height = img->h;

    SDL_Surface *new_img=SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    
    double rot_mat[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};
    
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            Uint32 old_pixel = get_pixel(img, j, i);
            double old_pixcos[] = {j, i};
            double new_pixcos[] = {0, 0}; 
            mul(rot_mat, old_pixcos, 2, 2, 1,  new_pixcos);
            /*if(new_pixcos[0] < img->w && new_pixcos[1] < img->h)
            {*/
                Uint8 r, g, b;
                SDL_GetRGB(old_pixel, img->format, &r, &g, &b);
                Uint32 newpixel = SDL_MapRGB(img->format, r, g, b);
                replace_pixel(new_img, i, j, newpixel);
            //}
        }
    }
    //foreach pixel SDL_MapRGB then SDL_putpixel
    SDL_SaveBMP(new_img, "image_rotated.jpeg");
}
