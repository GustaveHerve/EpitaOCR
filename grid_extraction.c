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

int avg_size(Square *sq, int len){

	double sum = 0;
	for (int i = 0; i < len; i++)
		sum += sq[i].NE.x - sq[i].NW.x;
	return sum / len;
}

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

Square *get_squares_seg(Segment *grid){

	Square *res = calloc(81, sizeof(Square));
	int index = 0;
	int xdis = grid[0].pt2.x - grid[0].pt1.x;
	int ydis = grid[10].pt2.y - grid[10].pt1.y;
	int xoffset = xdis / 9;
	int yoffset = ydis / 9;

	int start_x = grid[0].pt1.x;
	int start_y = grid[10].pt1.y;

	int posx = start_x;
	int posy = start_y;

	for (int i = 0; i < 9; i++){

		posx = start_x;
		for (int j = 0; j < 9; j++){

			TupleInt NW = {posx, posy};
			TupleInt SW = {posx, posy + yoffset};
			TupleInt NE = {posx + xoffset, posy};
			TupleInt SE = {posx + xoffset, posy + yoffset};

			Square s = { NW, SW, NE, SE };
			res[index] = s;
			index++;

			posx += xoffset;
		}
		posy += yoffset;
	}

	return res;

}
void save_squares_seg(Square *sq, SDL_Surface *image, char* path){

	int avg = avg_size(sq, 81);

	for (int i = 0; i < 81; i++){

		char *name = malloc(3 * sizeof(char));
        name[2] = 0;
		if (i < 10)
			name[0] = '0';
		else
		{
			int temp = i / 10;
			name[0] = temp + '0';
		}
		char *second = malloc(2 * sizeof(char));
	    name[1] = (i % 10) + '0';
		//int w = sq[i].NE.x - sq[i].NW.x;
		//int h = sq[i].SW.y - sq[i].NW.y;
		int w = avg;
		int h = avg;

		SDL_Surface* crop = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
		SDL_Surface* temp = SDL_CreateRGBSurface(0, avg, avg, 32, 0, 0, 0, 0);
		SDL_Rect rect;
		rect.x = sq[i].NW.x;
		rect.y = sq[i].NW.y;
		rect.w = w;
		rect.h = h;

		SDL_BlitSurface(image, &rect, temp, NULL);

		SDL_BlitScaled(temp, NULL, crop, NULL);
		char *ext = ".png";
		char *pathres = calloc(50 ,sizeof(char));
		strcat(pathres, path);
		strcat(pathres, name);
		strcat(pathres, ext);
    	IMG_SavePNG(crop, pathres);
	}
}

void save_squares(Square *sq, int len, SDL_Surface *image){

	int avg = avg_size(sq, len);

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

		//int w = sq[i].NE.x - sq[i].NW.x;
		//int h = sq[i].SW.y - sq[i].NW.y;
		int w = avg;
		int h = avg;

		SDL_Surface* crop = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
		SDL_Surface* temp = SDL_CreateRGBSurface(0, avg, avg, 32, 0, 0, 0, 0);
		SDL_Rect rect;
		rect.x = sq[i].NW.x;
		rect.y = sq[i].NW.y;
		rect.w = w;
		rect.h = h;

		SDL_BlitSurface(image, &rect, temp, NULL);
		dilate(temp, 3);

		SDL_BlitScaled(temp, NULL, crop, NULL);
		char *path1 = "/Users/gustave/Documents/c/grid/";
		char *ext = ".png";
		char *path = calloc(50, sizeof(char));
		strcat(path, path1);
		strcat(path, name);
		strcat(path, ext);
    	IMG_SavePNG(crop, path);
	}
}
