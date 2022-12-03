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
    return numbir < 0 ? -numbir:numbir;
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
    new_img = SDL_ConvertSurfaceFormat(new_img, img->format->format, 0);
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
                pixels[i * width + j] = old_pixel;
            }
        }
    }
    IMG_SavePNG(new_img, "image_rotated.png");
}


double* get_hMatrix(int* old, int* new)
{
    //old = corners of the image given in arguments
    //new = corners of the resulting image

    double matrix[81] = {-old[0], -old[1], -1, 0, 0, 0, old[0]*new[0], 
                                        old[1]*new[0], new[0],
                          0, 0, 0, -old[0], -old[1], -1, old[0]*new[1],
                                        old[1]*new[1], new[1], 
                          -old[2], -old[3], -1, 0, 0, 0, old[2]*new[2], 
                                        old[3]*new[2], new[2],
                          0, 0, 0, -old[2], -old[3], -1, old[2]*new[3], 
                                        old[3]*new[3], new[3], 
                          -old[4], -old[5], -1, 0, 0, 0, old[4]*new[4], 
                                        old[5]*new[4], new[4], 
                          0, 0, 0, -old[4], -old[5], -1, old[4]*new[5],
                                        old[5]*new[5], new[5], 
                          -old[6], -old[7], -1, 0, 0, 0, old[6]*new[6], 
                                        old[7]*new[6], new[6], 
                          0, 0, 0, -old[6], -old[7], -1, old[6]*new[7],
                                        old[7]*new[7], new[7], 
                                      0, 0, 0, 0, 0, 0, 0, 0, 1};

    //print_matrix("matrix 9*9 base: ", matrix, 9, 9);
    double col[9] = {0, 0, 0, 0, 0, 0, 0, 0, 1};

    double* inverse = inverseMat(matrix, 9);
   
    //print_matrix("matrix inverted: ", inverse, 9, 9);
    double* hmatrixc = (double*) malloc(9*sizeof(double));
    mul(inverse, col, 9, 9, 3, hmatrixc);
   
    print_matrix("after mul: ", hmatrixc, 9, 1);
    //transforming our column matrix into a 3*3 square matrix 
    double* hmatrix = (double*) malloc(9*sizeof(double));
    int k = 0;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            hmatrix[i*3+j] = hmatrixc[k];
            k++;
        }
    }
    hmatrix = inverseMat(hmatrix, 3);

    free(hmatrixc);
    return hmatrix; 
}

void homographic_Transform(SDL_Surface *img, Square corners)
{
    //below are the coordinates of the grid given in arguments
    int x1 = corners.NW.x;
    int y1 = corners.NW.y;

    int x2 = corners.NE.x;
    int y2 = corners.NE.y;

    int x3 = corners.SE.x;
    int y3 = corners.SE.y;

    int x4 = corners.SW.x;
    int y4 = corners.SW.y;
    
    //find size of the image we will get after homographic transform

    int a = sqrt((x2-x1) * (x2-x1) + (y1-y2) * (y1-y2)); 
    int b = sqrt((x2-x3) * (x2-x3) + (y2-y3) * (y2-y3));
    int c = sqrt((x4-x3) * (x4-x3) + (y3-y4) * (y3-y4));
    int d = sqrt((x1-x4) * (x1-x4) + (y1-y4) * (y1-y4));
    double size = fmax(fmax(a,b), fmax(c,d)); 
   
    int grid_corners[8] = {x1, y1, x2, y2, x3, y3, x4, y4};
    int newGrid_corners[8] = {0, 0, size, 0, size, size, 0, size};

    double* hmatrix = get_hMatrix(grid_corners, newGrid_corners);
    double* newCos = malloc(sizeof(double)*3);
    
    SDL_Surface *new_img= SDL_CreateRGBSurface(0,size,size,32,0,0,0,0);
    Uint32* pixels = new_img->pixels;
    Uint32* oldpixels = img->pixels;
    
    double h = img->h;
    double w = img->w;

    for(int y = 0; y < h; y++)
    {
       for(int x = 0; x < w; x++)
       {
            double pixelC[3] = {x, y, 1};
            mul(hmatrix, pixelC, 3, 3, 1, newCos); 
            
            double new_x = newCos[0]/newCos[2];
            double new_y = newCos[1]/newCos[2];

            if(new_x >= 0 && new_x <= size && new_y >= 0 && new_y <=size)
            {
                Uint32 old_pixel = oldpixels[(int)(new_y * w + new_x)];
                pixels[y * (int)w +x] = old_pixel;
            }                  
       } 
    }
    free(newCos);
    free(hmatrix);

    IMG_SavePNG(new_img, "homographic_image.png");
}







void homographicT(SDL_Surface *img, Square corners) //original version
{
    //below are the coordinates of the grid given in arguments
    int x1 = corners.NW.x;
    int y1 = corners.NW.y;

    int x2 = corners.NE.x;
    int y2 = corners.NE.y;

    int x3 = corners.SW.x;
    int y3 = corners.SW.y;

    int x4 = corners.SE.x;
    int y4 = corners.SE.y;
    
//let's find the size of the image we will get after homographic transform

    int a = sqrt((x2-x1) * (x2-x1) + (y1-y2) * (y1-y2));
    int b = sqrt((x2-x4) * (x2-x4) + (y2-y4) * (y2-y4));
    int c = sqrt((x4-x3) * (x4-x3) + (y3-y4) * (y3-y4));
    int d = sqrt((x1-x3) * (x1-x3) + (y1-y3) * (y1-y3));
    
    printf("a = %d\n b = %d\n c = %d\n d = %d\n", a, b, c, d);
    int size = fmax(fmax(a,b), fmax(c,d)); 
    printf("size of new_img is: %d", size); 

//here are the coordinates of the points above ((x1,y1), (x2,y2), ...) in the new image

    int x1bis = 0;
    int y1bis = 0;

    int x2bis = size;
    int y2bis = 0;

    int x3bis = 0;
    int y3bis = size;

    int x4bis = size;
    int y4bis = size;

//initializing the matrix that will allow us to compute the coordinates of each pixel

    //double *mat = malloc(sizeof(double)*64);
    double mat[64] = {x1, y1, 1, 0, 0, 0, (int)-x1*x1bis, (int)-y1*x1bis,
                       0, 0, 0, x1, y1, 1, (int)-x1*y1bis, (int)-y1*y1bis,
                      x2, y2, 1, 0, 0, 0, (int)-x2*x2bis, (int)-y2*x2bis,
                       0, 0, 0, x2, y2, 1, (int)-x2*y2bis, (int)-y2*y2bis,
                      x3, y3, 1, 0, 0, 0, (int)-x3*x3bis, (int)-y3*x3bis,
                       0, 0, 0, x3, y3, 1, (int)-x3*y3bis, (int)-y3*y3bis,
                      x4, y4, 1, 0, 0, 0, (int)-x4*x4bis, (int)-y4*x4bis, 
                       0, 0, 0, x4, y4, 1, (int)-x4*y4bis, (int)-y4*y4bis};
   
    //double* matp = realloc(mat, sizeof(double)*64);
    double (*matp)[64] = &mat;

    print_matrix("matp", *matp, 8, 8); //*matp
    
    double ncos[8]= {x1bis, y1bis, x2bis, y2bis, x3bis, y3bis,x4bis,y3bis};

    double (*pncos)[8] = &ncos;


//Now let's resolve the system to find the values of the constants above

    double* invertedMat = malloc(sizeof(double)*64);
    invertedMat = inverseMat(*matp/*, invertedMat*/, 8); //*matp
    print_matrix("invertedMat=\n", invertedMat, 8, 8);
    double* r = malloc(sizeof(double)*8);
    mul(invertedMat, *pncos, 8, 8, 1, r); //mul stores the result in r

    int a1 = r[0];
    int a2 = r[1];
    int a3 = r[2];
    int b1 = r[3];
    int b2 = r[4];
    int b3 = r[5];
    int c1 = r[6];
    int c2 = r[7];

    
    double cons[8] = {a1, a2, a3, b1, b2, b3, c1, c2};
    double (*kmat)[8] = &cons;

    print_matrix("constantsMat", *kmat, 8, 1);
    //we can now use these constants to resolve a system
    
    double width = img -> w;
    double height = img -> h;

    SDL_Surface *new_img= SDL_CreateRGBSurface(0,size,size,32,0,0,0,0);
    Uint32* pixels = new_img->pixels;    

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; i < width; j++)
        {
            int new_x = (a1*i+a2*j+a3) / (c1*i+c2*j+1);
            int new_y = (b1*i+b2*j+b3) / (c1*i+c2*j+1);

            if(new_x >= 0 && new_y >= 0 && new_x<size && new_y<size)
            {
                Uint32 old_pixel = get_pixel(img, new_x, new_y);
                pixels[j*(int)width+i] = old_pixel; //i*width+j
            }

        }
    }
    IMG_SavePNG(new_img, "homographic_image.png");
}

