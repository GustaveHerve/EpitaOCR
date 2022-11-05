#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "include/pixel.h"
#include "include/edge_detection.h"
#include "include/image_processing.h"
#include "include/image_loading.h"
#include "include/matrix.h"
#include "include/utils.h"
#include "include/geometry.h"
#include "include/grid_detection.h"
#include "include/morph.h"
#include "include/thresholding.h"
#include <string.h>

int get_squares(Line *x, Line *y, Square *res){

	int len = 0;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			Square sq;
			TupleInt pt;

			polar_intersection(&pt, x[i], y[j]);
			sq.NW = pt;
			polar_intersection(&pt, x[i], y[j+1]);
			sq.NE = pt;
			polar_intersection(&pt, x[i+1], y[j]);
			sq.SW = pt;
			polar_intersection(&pt, x[i+1], y[j+1]);
			sq.SE = pt;

			res[len] = sq;
			len++;
        }
	}

	return len;
}

void save_squares(Square *sq, int len, SDL_Surface *image){

	for (int i = 0; i < len; i++){

		char *name = malloc(3 * sizeof(char));
		if (i < 10)
			name[0] = '0';
		else
		{
			int temp = i / 10;
			name[0] = temp + '0'; 
		}

		char *second = malloc(2 * sizeof(char));
	    second[0] = (i % 10) + '0';
		strcat(name, second);

		int w = sq[i].NE.x - sq[i].NW.x;
		int h = sq[i].SW.y - sq[i].NW.y;

		SDL_Surface* crop = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
		SDL_Rect rect;
		rect.x = sq[i].NW.x;
		rect.y = sq[i].NW.y;
		rect.w = w;
		rect.h = h;

		SDL_BlitSurface(image, &rect, crop, NULL);
		char *path1 = "/Users/gustave/Documents/c/grid/";
		char *ext = ".png";
		char *path = malloc(50 * sizeof(char));
		strcat(path, path1);
		strcat(path, name);
		strcat(path, ext);
    	IMG_SavePNG(crop, path);
	}
}



