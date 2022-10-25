#include <stdlib.h>
#include "include/utils.h"
#include "include/pixel.h"
#include "include/image_processing.h"

void dilate_c(SDL_Surface *image, int kersize, int r[]){

	int width = image->w;
	int height = image->h;
	int len = height * width;

	Uint32 *pixels = malloc(sizeof(Uint32) * len);
	get_pixel_array(image, pixels);

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			double acc = 0;
			int roff = kersize/2;
			int coff = kersize/2;

			for (int k = -1 * roff; k <= roff; k++){
				for (int l = -1 * coff; l <= roff; l++){
					if (i+k >= 0 && j+l >= 0 && i+k <height && j+l < width){
						Uint8 value;
						SDL_GetRGB(pixels[(i+k)*width+(j+l)], image->format,
							   	&value, &value, &value);
						if (value > acc)
							acc = value;
					}
				}
			}
			r[i*width + j] = acc;
		}
	}
	free(pixels);
}

void dilate(SDL_Surface *image, int kersize){

	int *arr = calloc(image->w * image->h, sizeof(int));
	dilate_c(image, kersize, arr); 
	apply_convolution_int(image, arr, image->h, image->w);
	free(arr);

}

void erose_c(SDL_Surface *image, int kersize, int r[]){

	int width = image->w;
	int height = image->h;
	int len = height * width;

	Uint32 *pixels = malloc(sizeof(Uint32) * len);
	get_pixel_array(image, pixels);

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			double acc = 0;
			int roff = kersize/2;
			int coff = kersize/2;
			int first = 1;

			for (int k = -1 * roff; k <= roff; k++){
				for (int l = -1 * coff; l <= roff; l++){
					if (i+k >= 0 && j+l >= 0 && i+k < height && j+l < width){
						Uint8 value;
						SDL_GetRGB(pixels[(i+k)*width+(j+l)], image->format,
							   	&value, &value, &value);
						if (first){
							acc = value;
							first = 0;
							continue;
						}

						if (value < acc)
							acc = value;
					}
					
				}
			}
			r[i*width + j] = acc;
		}
	}
	free(pixels);
}

void erose(SDL_Surface *image, int kersize){

	int *arr = calloc(image->w * image->h, sizeof(int));
	erose_c(image, kersize, arr); 
	apply_convolution_int(image, arr, image->h, image->w);
	free(arr);

}

void opening(SDL_Surface *image, int kersize){

	erose(image, kersize);
	dilate(image, kersize);

}

void closing(SDL_Surface *image, int kersize){

	dilate(image, kersize);
	erose(image, kersize);
}
