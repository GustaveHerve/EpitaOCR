#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/utils.h"
#include "include/geometry.h"
#include "include/matrix.h"
#include "include/image_processing.h"

void greyscale(SDL_Surface *image)
{
	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			Uint32 newpixel = SDL_MapRGB(image->format, average, average,
					average);

			replace_pixel(image, j, i, newpixel);

		}
	}
}

void gauss_ker(int size, double sigma, double ker[])
{
	double r, s = 2.0 * sigma * sigma;
	double sum;

	int beg = size/2;
	for (int x = -beg; x <= beg; x++)
	{
		for (int y = -beg; y <= beg; y++)
		{
			r = sqrt(x * x + y * y);
			ker[(x+beg) * size + y + beg] = (exp(-(r * r) / s)) / (M_PI * s);
			sum += ker[(x+beg) * size + y + beg];
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			ker[i*size + j] /= sum;
	}

}

void gauss_blur(SDL_Surface* img, int size, double sigma)
{
    int *b = malloc(sizeof(int) * img->w * img->h);
	double *ker = calloc(size*size, sizeof(double));
	gauss_ker(size, sigma, ker);
	blur_convolution(img, ker, size, size, b);
	apply_convolution_int(img, b, (size_t) img->h, (size_t) img->w);
	free(b);
}

void blur(SDL_Surface* image, int kersize)
{
    int *b = malloc(sizeof(int) * image->w * image->h);
    if (b == NULL)
        errx(1, "blur_c: couldn't allocate memory for blurring");
        
    switch(kersize){

        case 3:
			{
    		double blur[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 
				0.0625, 0.125, 0.0625};
            convolution(image, blur, 3, 3, b, 1);
            break;}
				

        case 5:
			{
            double blur[] = {
				1.0/256, 4.0/256, 6.0/256, 4.0/256, 1.0/256, 
				4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256,
			   	6.0/256, 24.0/256, 36.0/256, 24.0/256, 6.0/256, 
				4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256, 
				1.0/256, 4.0/256, 6.0/256, 4.0/256, 1.0/256};
			blur_convolution(image, blur, 5, 5, b);
			break;}

		default:
			errx(2, "blur_c: invalid kersize");

    }

	apply_convolution_int(image, b, (size_t)image->h, (size_t)image->w);
    free(b);

}

//apply_convolution: applies the result of a convolution (stored in r[]) to 
//an SDL_Surface image 
//rows and cols determines r[]'s size
void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, 
		size_t cols){
   
    for (size_t i = 0; i < rows; i++){
        for (size_t j= 0; j < cols; j++){
            Uint32 pixel = 
				SDL_MapRGB(image->format, r[i*cols + j], r[i*cols + j],
					r[i*cols + j]);
            replace_pixel(image, j, i, pixel);
        }
    }
}

//apply_convolution_int: same as previous but accepts int array and clamp 
//between 0 and 255 to avoid
//overflow
void apply_convolution_int(SDL_Surface *image, int r[], size_t rows, 
		size_t cols){
   
    for (size_t i = 0; i < rows; i++){
        for (size_t j= 0; j < cols; j++){

			Uint8 value;
			if (r[i*cols+j] >= 255)
				value = 255;
			else if (r[i*cols+j] < 0)
				value = 0;
			else
				value = (Uint8)r[i*cols+j];

            Uint32 pixel = SDL_MapRGB(image->format, value, value,
					value);
            replace_pixel(image, j, i, pixel);
        }
    }
}

//invert each pixel from a binary image
void invert(SDL_Surface *image){
	int w = image->w;
	int h = image->h;
	Uint32 black = SDL_MapRGB(image->format, 0, 0, 0);
	Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 val = 0;
			SDL_GetRGB(pixel, image->format, &val, &val, &val);
			if (val == 255)
				replace_pixel(image, j, i, black);
			else if (val == 0)
				replace_pixel(image, j, i, white);
		}
	}
}
