#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/matrix.h"
#include "include/geometry.h"


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

    Uint32* pixels = new_img->pixels;

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
                //replace_pixel(new_img, x2, y2, old_pixel);
		int c = x2 * width + y2;
		pixels[c] = old_pixel;
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
    Uint32* pixels = new_img->pixels;
    for(long i = 0; i < height; i++)
    {
        for(long j = 0; j < width; j++)
        {

            long x2 = cosinus * (j - x0) - sinus * (i - y0) + x0;
            long y2 = sinus * (j - x0) + cosinus * (i - y0) + y0;

            if(x2 >= 0 && y2 >= 0 && x2<width && y2<height)
            {
                Uint32 old_pixel = get_pixel(img, x2, y2);
		pixels[i*width + j] = old_pixel;
            }
        }
    }
    IMG_SavePNG(new_img, "image_rotated.png");
}

void homographicT(SDL_Surface *img, Square corners)
{
    //below are the coordinates of the grid given in arguments
    double x1 = corners.NW.x;
    double y1 = corners.NW.y;

    double x2 = corners.NE.x;
    double y2 = corners.NE.y;

    double x3 = corners.SW.x;
    double y3 = corners.SW.y;

    double x4 = corners.SE.x;
    double y4 = corners.SE.y;
    
//let's find the size of the image we will get after homographic transform

    double a = sqrt((x2-x1) * (x2-x1) + (y1-y2) * (y1-y2));
    double b = sqrt((x2-x4) * (x2-x4) + (y2-y4) * (y2-y4));
    double c = sqrt((x4-x3) * (x4-x3) + (y3-y4) * (y3-y4));
    double d = sqrt((x1-x3) * (x1-x3) + (y1-y3) * (y1-y3));
    
    printf("a = %lf\n b = %lf\n c = %lf\n d = %lf\n", a, b, c, d);
    double size = fmax(fmax(a,b), fmax(c,d)); //convert to int if pb
    

//here are the coordinates of the points above ((x1,y1), (x2,y2), ...) in the new image

    double x1bis = 0;
    double y1bis = 0;

    double x2bis = size;
    double y2bis = 0;

    double x3bis = 0;
    double y3bis = size;

    double x4bis = size;
    double y4bis = size;

//initializing the matrix that will allow us to compute the coordinates of each pixel

    double mat[64] = {x1, y1, 1, 0, 0, 0, -x1*x1bis, -y1*x1bis,
                      0, 0, 0, x1, y1, 1, -x1*y1bis, -y1*y1bis,
                      x2, y2, 1, 0, 0, 0, -x2*x2bis, -y2*x2bis,
                      0, 0, 0, x2, y2, 1, -x2*y2bis, -y2*y2bis,
                      x3, y3, 1, 0, 0, 0, -x3*x3bis, -y3*x3bis,
                      0, 0, 0, x3, y3, 1, -x3*y3bis, -y3*y3bis,
                      x4, y4, 1, 0, 0, 0, -x4*x4bis, -y4*x4bis, 
                      0, 0, 0, x4, y4, 1, -x4*y4bis, -y4*y4bis};
    
    double (*matp)[64] = &mat;

    print_matrix("matp", *matp, 8, 8);
    
    double ncos[8]= {x1bis, y1bis, x2bis, y2bis, x3bis, y3bis,x4bis,y3bis};

    double (*pncos)[8] = &ncos;


//Now let's resolve the system to find the values of the constants above

    double* invertedMat = malloc(sizeof(double)*64);
    invertedMat = inverseMat(*matp, invertedMat, 8);

    double* r = malloc(sizeof(double)*8);
    mul(invertedMat, *pncos, 8, 8, 1, r);

    double a1 = r[0];
    double a2 = r[1];
    double a3 = r[2];
    double b1 = r[3];
    double b2 = r[4];
    double b3 = r[5];
    double c1 = r[6];
    double c2 = r[7];

    print_matrix("constantsMat", r, 8, 1);
    
    double cons[8] = {a1, a2, a3, b1, b2, b3, c1, c2};
    double (*kmat)[8] = &cons;

    //we can now use these constants to resolve a system
    
    double width = img -> w;
    double height = img -> h;

    SDL_Surface *new_img= SDL_CreateRGBSurface(0,size,size,32,0,0,0,0);
    Uint32* pixels = new_img->pixels;    

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; i < width; j++)
        {
            double new_x = (a1*i+a2*j+a3) / (c1*i+c2*j+1);
            double new_y = (b1*i+b2*j+b3) / (c1*i+c2*j+1);

            if(new_x >= 0 && new_y >= 0 && new_x<size && new_y<size)
            {
                Uint32 old_pixel = get_pixel(img, new_x, new_y);
                pixels[i*(int)width+j] = old_pixel;
            }

        }
    }
    IMG_SavePNG(new_img, "homographic_image.png");
}
