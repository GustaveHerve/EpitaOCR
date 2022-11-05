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

void adaptive_thresholding_c(SDL_Surface *image, int size, int *r, int c){

	SDL_LockSurface(image);
	Uint32 *pixels = image->pixels;
	int width = image->w;
	int height = image->h;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			int sum = 0;
			int roff = size/2;
			int count = 0;
			Uint8 curr_val = 0;
			SDL_GetRGB(pixels[i*width + j], image->format,
							   	&curr_val, &curr_val, &curr_val);

			for (int k = -roff; k <= roff; k++){
				for (int l = -roff; l <= roff; l++){

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

void adaptive_thresholding(SDL_Surface *image, int size, int c){

	int *r = calloc(image->w * image->h, sizeof(int));
	adaptive_thresholding_c(image, size, r, c);
	apply_convolution_int(image, r, image->h, image->w);
}

//void local_otsu(SDL_Surface *image
