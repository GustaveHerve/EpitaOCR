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

long Abs(long numbir)
{
    return numbir<0 ? -numbir:numbir;
}

void rotate_img(SDL_Surface *img, double angle)
{
    double width = img ->w;
    double height = img->h;   
    
    angle = deg_to_rad(angle);

    double y0 = ((height+1)/2)-1; 
    //coordinates of the center of the initial img
    double x0 = ((width+1)/2)-1;   
   
    double cosinus = cos(angle);   
    double sinus = sin(angle);
    double tangent = tan(angle/2);

    double nwidth = Abs(width*cos(angle)) +Abs(height*sin(angle))+1;  
    //new dimensions of the image
    double nheight = Abs(height*cos(angle)) + Abs(width*sin(angle))+1;  

    long ncenterx = ((nwidth+1)/2)-1;  
    long ncentery = ((nheight+1)/2)-1; 
    //coordinates of the center of the resulting image

    SDL_Surface *new_img= SDL_CreateRGBSurface(0,nwidth,nheight,32,0,0,0,0);
    
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
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
            }

        }
    }
    IMG_SavePNG(new_img, "image_rotated.png");
}

void rotate_img90(SDL_Surface *img, double angle)
{

    long width = img ->w;
    long height = img->h;
    
    angle = -deg_to_rad(angle);
    
    long y0 = height / 2;
    long x0 = width / 2;
   
    double cosinus = cos(angle);
    double sinus = sin(angle);

    SDL_Surface *new_img= SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    
    for(long i = 0; i < height; i++)
    {
        for(long j = 0; j < width; j++)
        {
            
            long x2 = cosinus * (i - x0) - sinus * (j - y0) + x0;
            long y2 = sinus * (i - x0) + cosinus * (j - y0) + y0;

            if(x2 >= 0 && y2 >= 0 && x2<width && y2<height)
            {
                Uint32 old_pixel = get_pixel(img, x2, y2);
                replace_pixel(new_img, i, j, old_pixel);
            }
        }
    }
    IMG_SavePNG(new_img, "image_rotated.png");
}
