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

//threshold: applies a fixed threshold treatment to a SDL_Surface
void threshold(SDL_Surface* image, float threshold)
{
	unsigned int width = image->w;
	unsigned int height = image->h;
	Uint8 t = threshold * 255;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 res = 0;
			if (r >= t)
				res = 255;

			Uint32 newpixel = SDL_MapRGB(image->format, res, res, res);
			replace_pixel(image, j, i, newpixel);

		}
	}
}

void threshold_value(SDL_Surface* image, int threshold)
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
			Uint8 res = 0;
			if (r > threshold)
				res = 255;

			Uint32 newpixel = SDL_MapRGB(image->format, res, res, res);
			replace_pixel(image, j, i, newpixel);
		}
	}
}

//Compute otsu global threshold and returnx it without applying it
float otsu_threshold(SDL_Surface* image){

	int n = image->w * image->h;
	float thresh = 0, var_max = 0, sum = 0, sumB = 0;
	float q1 = 0, q2 = 0, u1 = 0, u2 = 0;
	int max_intensity = 255;

	Histo hist;
	histo_init(&hist);
	histo_compute(image, &hist);


	for (int i = 0; i <= max_intensity; i++){
		sum += (float)i * (float)hist.values[i];
	}

	for (int t = 0; t <= max_intensity; t++){
		q1 += (float)hist.values[t];
		if (q1 == 0){
			continue;
		}
		q2 = (float)n - q1;

		sumB += t * hist.values[t];
		u1 = sumB / q1;
		u2 = (sum - sumB) / q2;

		float var = q1 * q2 * ((u1 - u2) * (u1 - u2));

		if (var > var_max){
			thresh = t;
			var_max = var;
		}

	}
	return thresh/max_intensity;

}

//Computes otsu threshold and apply it
void otsu(SDL_Surface* image)
{
	float thresh = otsu_threshold(image);
	threshold_value(image, thresh*255);
}

void adaptive_thresholding_c(SDL_Surface *image, int size, int *r, int c)
{
	SDL_LockSurface(image);
	Uint32 *pixels = image->pixels;
	int width = image->w;
	int height = image->h;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int sum = 0;
			int roff = size/2;
			int count = 0;
			Uint8 curr_val = 0;
			SDL_GetRGB(pixels[i*width + j], image->format,
							   	&curr_val, &curr_val, &curr_val);

			for (int k = -roff; k <= roff; k++)
			{
				for (int l = -roff; l <= roff; l++)
				{
					//Checks that we're not out of range
					if (i+k >= 0 && j+l >= 0 && i+k <height && j+l < width){
						Uint8 value = 0;
						SDL_GetRGB(pixels[(i+k)*width+(j+l)], image->format,
							   	&value, &value, &value);
						sum += value;
						count++;
					}

				}
			}

			Uint8 threshold = sum / count - c;
			if (curr_val > threshold)
				r[i*width + j] = 255;
			else
				r[i*width + j] = 0;

		}
	}

	SDL_UnlockSurface(image);
}

void adaptive_thresholding(SDL_Surface *image, int size, int c)
{
	int *r = calloc(image->w * image->h, sizeof(int));
	adaptive_thresholding_c(image, size, r, c);
	apply_convolution_int(image, r, image->h, image->w);
}

void adaptive_gaussthresholding_c(SDL_Surface *image, int size, int *r, int c)
{
	double *ker = calloc(size*size, sizeof(double));
	gauss_ker(size, 1.5, ker);

	SDL_LockSurface(image);
	Uint32 *pixels = image->pixels;
	int width = image->w;
	int height = image->h;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double sum = 0;
			int roff = size/2;
			int count = 0;
			Uint8 curr_val = 0;
			SDL_GetRGB(pixels[i*width + j], image->format,
							   	&curr_val, &curr_val, &curr_val);

			for (int k = -roff; k <= roff; k++)
			{
				for (int l = -roff; l <= roff; l++)
				{
					//Checks that we're not out of range
					if (i+k >= 0 && j+l >= 0 && i+k <height && j+l < width){
						Uint8 value = 0;
						SDL_GetRGB(pixels[(i+k)*width+(j+l)], image->format,
							   	&value, &value, &value);
						sum += (double)value * ker[(k+roff) * size + l+roff];
						count++;
					}

				}
			}

			Uint8 threshold = (int)sum - c;
			if (curr_val > threshold)
				r[i*width + j] = 0;
			else
				r[i*width + j] = 255;

		}
	}
	free(ker);
	SDL_UnlockSurface(image);
}

void adaptive_gaussthresholding(SDL_Surface *image, int size, int c)
{
	int *r = calloc(image->w * image->h, sizeof(int));
	adaptive_gaussthresholding_c(image, size, r, c);
	apply_convolution_int(image, r, image->h, image->w);
}


