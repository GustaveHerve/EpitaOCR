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

void hough_lines_gradient(SDL_Surface* image, int angleNb, Uint8 *edges, Uint8 *angles, int res[]){
	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){
			int c = i*width + j;
			if (edges[c] == 255){
				float rad = angles[c] * M_PI / 180;
				float rho = j*cos(rad) + i*sin(rad);
				if (rho < 0)
					continue;
				unsigned int rhoi = roundf(rho);
				res[rhoi * angleNb + angles[c]]++;

			}
		}
	}
}

/*  
void hough_lines_local(SDL_Surface* image, int angleNb, int ker, int res[]){
	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			int off = ker/2;
			float maxrho;
			float maxangle;
			float rad = 

			for (unsigned int k = -off; k <= off; k++){
				for (unsigned int l = -off; l <= off; l++){
					if (i+k < height && i+k >= 0 && j+l >= 0 && j+l < width){

					}


				}
			}


		}
	}
}
*/
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

void neighbour_suppr(int *hough, int rows, int cols, int r, int theta){
	int roff = rows/4;
	int coff = cols/4;
	if (roff % 2 == 0)
		roff++;
	if (coff % 2 == 0)
		coff++;

	for (int k = -roff; k <= roff; k++){
		for (int l = -coff; l <= coff; l++){
			if (r+k >= 0 && r+k < rows && l+theta >= 0 && l+theta < cols){
				hough[(k+r)*cols + l+theta] = 0;
			}
		}
	}
}

TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, int step, int tol, Line hor[], Line ver[]){

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
				if (jmax <= 0 + tol || jmax >= 360 - tol){
					ver[veracc] = new;
					veracc++;
				}
				else if ( (jmax >= 90 - tol && jmax <= 90 + tol ) || (jmax >=  270 - tol && jmax <= 270 + tol)){
					hor[horacc] = new;
					horacc++;
				}
				//neighbour_suppr(input, rows, cols, i, j);
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

int merge_lines(Line* lines, int len, int thresh, Line* res){
	if (len <= 0)
		return -1;
	int curdist = 0;
	int index = 0;
	Line max = lines[0];

	for (int i = 1; i < len; i++){
		curdist += lines[i].rho - lines[i-1].rho;
		if (curdist > thresh){
			res[index] = max;
			index++;
			max = lines[i];
			curdist = 0;
			continue;
		}

		if (lines[i].rho > max.rho)
			max = lines[i];
	}

	if (max.rho - res[index-1].rho > thresh)
		res[index] = max;

	return index+1; 
	
}

int average_dist(int* dists, int len){

	int sum = 0;
	for (int i = 0; i < len; i++){
		sum += dists[i];
	}
	return sum / len;

}

int get_grid_lines(Line* lines, int len, int* dis, int tolerance, Line* res){

	int res_dist = 0;
	int cur_dist = dis[0];
	int res_count = 0;
	int cur_count = 1;

	//int avg = average_dist(dis, len-1);

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

	if (len < 2)
		return 0;
	int* gX = malloc(sizeof(int) * (len-1));
	line_distances(lines, len, gX);
	int n = get_grid_lines(lines, len, gX, tolerance, res);
	free(gX);
	return n;

}
