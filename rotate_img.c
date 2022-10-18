#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/matrix.h"

double deg_to_rad(double angle)
{
    return angle*(M_PI/180.0);
}
void rotate_img(SDL_Surface *img, double angle)
{
    double width = img ->w;
    double height = img->h;   //remettre en long après si fail
    
    angle = deg_to_rad(angle);

    double y0 = ((height+1)/2)-1;
    double x0 = ((width+1)/2)-1;
   
    double cosinus = cos(angle);
    double sinus = sin(angle);
    double tangent = tan(angle/2);

    double nwidth = fabs(width*cos(angle)) +fabs(height*sin(angle))+1;
    double nheight =fabs(height*cos(angle)) +fabs(width*sin(angle))+1;

    long ncenterx = ((nwidth+1)/2)-1;
    long ncentery = ((nheight+1)/2)-1;

    SDL_Surface *new_img= SDL_CreateRGBSurface(0,nwidth,nheight,32,0,0,0,0);
    
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            //Uint32 old_pixel = get_pixel(img, j, i);

            //long x2 = cosinus * ((double)i - x0) - sinus * ((double)j - y0) + x0;
            //long y2 = sinus * ((double)i - x0) + cosinus * ((double)j - y0) + y0;

            
            long y2 = height-1-i-y0;
            long x2 = width-1-j-x0;

            x2 = x2-(y2*tangent);       //Shear 1 
            y2 = x2*sin(angle)+y2;     //Shear 2
            x2 = x2-(y2*tangent);      //Shear 3

            y2 = ncentery-y2;
            x2 = ncenterx-x2;

            if(x2>0 && y2>0 && x2<nwidth && y2<nheight)
            {
                Uint32 old_pixel = get_pixel(img, j, i);
                replace_pixel(new_img, x2, y2, old_pixel);
                //replace_pixel(new_img, x2, y2, old_pixel);
            }

        }
    }
    IMG_SavePNG(new_img, "image_rotated.png");
}
