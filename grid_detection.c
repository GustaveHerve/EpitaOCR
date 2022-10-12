#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/utils.h"
#include "include/geometry.h"

//HOUGH TRANSFORM RELATED METHODS
void hough_init(int res[], int rows, int cols){

	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			res[i * cols + j] = 0;
		}
	}
}

void hough_lines(SDL_Surface* image, int angleNb, int step, int res[]){

	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r > 100)
			{
				for (int k = 0; k < angleNb; k+=step){
					
					float rad = k * M_PI / 180;
					float rho = j * cos(rad) + i * sin(rad);
					unsigned int rhoi = roundf(rho);
					//if (rhoi < 0)
						//continue;
					res[rhoi * angleNb + k]++; 
				
				}
			}
		}
	}
}

int hough_filter(int input[], int rows, int cols, int threshold, Line res[]){

	int acc = 0;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			if (input[i * cols + j] >= threshold){
				Line new = {i,j};
				res[acc] = new;
				acc++;
			}
		}
	}

	return acc;
}

TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, int step, Line hor[], Line ver[]){

	int horacc = 0;
	int veracc = 0;

	for (int i = 0; i < rows; i += step){
		for (int j = 0; j < cols; j += step){

			int imax = i;
			int jmax = j;

			for (int k = 0; k < step; k++){
				for (int l = 0; l < step; l++){

					if (i+k < rows && j+l < cols){
						if (input[(i+k)*cols + j+l] > input[imax*cols + jmax]){
							imax = i+k;
							jmax = j+l;
						}
					}
				}
			}

			if (input[imax * cols +  jmax] >= threshold){
				Line new;
				new.theta = jmax;
				new.rho = imax;
				if (jmax <= 5 || jmax >= 355 ){
					ver[veracc] = new;
					veracc++;
				}
				else if ( (jmax >= 85 && jmax <= 95 ) || (jmax >= 265 && jmax <= 275)){
					hor[horacc] = new;
					horacc++;
				}
			}
		}
	}

	TupleInt res;
	res.x = horacc;
	res.y = veracc;
	return res;
}


