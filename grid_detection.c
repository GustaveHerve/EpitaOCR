#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <err.h>
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
					if (rho < 0)
						continue;
					unsigned int rhoi = roundf(rho);
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

void line_distances(Line* lines, int len, int* res){

	if (res == NULL)
		errx(1, "line_distances: failed allocating memory ");
	for (int i = 1; i < len; i++)
		res[i-1] = lines[i].rho - lines[i-1].rho;

}

int get_grid_lines(Line* lines, int len, int* dis, int tolerance, Line* res){

	int res_dist = 0;
	int cur_dist = dis[0];
	int res_count = 0;
	int cur_count = 1;

	for (int i = 1; i < len -1; i++){

		if (dis[i] <= cur_dist + tolerance && dis[i] >= cur_dist - tolerance){
			cur_count++;

			if (i == len - 2){
				res_count = cur_count;
				res_dist = cur_dist;
			}
		}

		else{

			if (cur_count > res_count){
				res_count = cur_count;
				res_dist = cur_dist;
			}

			cur_count = 1;
			cur_dist = dis[i];

		}

	}

	if (res == NULL)
		errx(2, "get_grid_lines: failed allocating memory");
	int j = 0;
	for (int i = 0; i < len-1; i++){

		if (dis[i] <= res_dist + tolerance && dis[i] >= res_dist - tolerance){
			if (j == 0){
				res[j] = lines[i];
				res[j+1] = lines[i+1];
				j+= 2;
			}
			else{
				res[j] = lines[i+1];
				j++;
			}
		}
	}
	return j;

}

int get_grid(Line* lines, int len, int tolerance, Line* res){

	int* gX = malloc(sizeof(int) * (len-1));
	line_distances(lines, len, gX);
	int n = get_grid_lines(lines, len, gX, tolerance, res);
	free(gX);
	return n;

}
