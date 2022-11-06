#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/pixel.h"
#include "include/matrix.h"
#include "include/image_processing.h"
#include "include/edge_detection.h"
#include "include/thresholding.h"

#define WEAK 100
#define STRONG 255

/* gradient: computes edge and angle gradient values from two matrices of same size
*  stores the result in edges[] and angles[] (so it doesn't affect the image directly)
*/
void gradientv2(int r1[], int r2[], Uint8 edges[], Uint8 angles[], size_t rows, size_t cols){

	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){	
			int current = i*cols + j;
			double s1 = (double)r1[current] * (double)r1[current];
			double s2 = (double)r2[current] * (double)r2[current];
			double temp = sqrt(s1+s2);
			//double temp = fabs(r1[current]) + fabs(r2[current]); //faster compute but less precise
			if (temp > 255)
				temp = 255;
			edges[current] = (Uint8)temp;

			float tan = atan2(r2[current], r1[current]);
			float dtan = 0;
			dtan = tan * 180 / M_PI;
			if (dtan < 0)
				dtan += 180; //get opposite point in triogo circle for easier comparisons
							 //
			   
			Uint8 angle = 0;
			if ((dtan >= 0 && dtan < 22.5) || (dtan <= 180 && dtan >= 157.5)) 
				angle = 0;
			else if (dtan >= 22.5 && dtan < 67.5)
				angle = 45;
			else if (dtan >= 67.5 && dtan < 112.5)
				angle = 90;
			else if (dtan >= 112.5 && dtan < 157.5)
				angle = 135;

				
			angles[current] = angle;
		}	
	}
}
//Compute sobel and returns result in place in edges and angles
//useful for Canny
void sobel_c(SDL_Surface* surf, Uint8* edges, Uint8* angles){

	int *r1 = malloc(sizeof(int) * surf->w * surf->h);
    int *r2 = malloc(sizeof(int) * surf->w * surf->h);

	double kerx[] = { 1, 0, -1, 2, 0, -2, 1, 0, -1};
    double kery[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};  

	convolution(surf, kerx, 3, 3, r1, 1);
    convolution(surf, kery, 3, 3, r2, 1);

	gradientv2(r1, r2, edges, angles, surf->h, surf->w);

	free(r1);
    free(r2);

}

//Compute sobel and apply it immediately unlike sobel_c
//useful for testing sobel alone
void sobel(SDL_Surface* surf){

	Uint8 *edges = malloc(sizeof(Uint8) * surf->w * surf->h);
	Uint8 *angles = malloc(sizeof(Uint8) * surf->w * surf->h);
	sobel_c (surf, edges, angles);
    apply_convolution(surf, edges, (size_t)surf->h, (size_t)surf->w);

}

//Canny substep
void non_maxima_suppr(Uint8 edges[], Uint8 angles[], size_t rows, size_t cols,
		Uint8 res[]){

	for (int i = 0; i < (int)rows; i++){
		for (int j = 0; j < (int)cols; j++){
			unsigned int c = i * cols + j;
			Uint8 angle = angles[c];

			switch (angle)
			{

			case 0:
				if (j+1 < (int)cols && j-1 >= 0){
					if (!(edges[c] >= edges[c-1] && edges[c] >= edges[c+1]))
						res[c] = 0;
					else
						res[c] = edges[c];
				}
				break;

			case 45:
				if ((j+1 < (int)cols && i+1 < (int)rows) && (i-1 >= 0 && j-1 >= 0)){
					if (!(edges[c] >= edges[(i-1)*cols+j+1] && edges[c] >= edges[(i+1)*cols+j-1]))
						res[c] = 0;
					else
						res[c] = edges[c];
				}
				break;

			case 90:
				if (i+1 < (int)cols && i-1 >= 0){
					if (!(edges[c] >= edges[(i-1)*cols+j] && edges[c] >= edges[(i+1)*cols+j]))
						res[c] = 0;
					else
						res[c] = edges[c];
				}
				break;

			case 135:
				if ((j-1 >= 0 && i+1 < (int)rows ) && (j+1 < (int)cols && i-1 >= 0)){
					if (!(edges[c] >= edges[(i-1)*cols+j-1] && edges[c] >= edges[(i+1)*cols+j+1]))
						res[c] = 0;
					else
						res[c] = edges[c];
				}
				break;

					
			}

		}
	}
}

void double_thresholding(Uint8 *edges, size_t rows, size_t cols, 
		float lowratio, float highratio){

	Uint8 highvalue = highratio * 255;
	Uint8 lowvalue = highvalue * lowratio;

	Stack_Tint* s = newStack_Tint(rows*cols);

	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){
			int c = i * cols + j;
			if (edges[c] >= highvalue){
				edges[c] = STRONG;
				TupleInt e = {i,j}; 
				stackTint_push(s, e);
			}
			else if (edges[c] >= lowvalue)
				edges[c] = WEAK;
			
			else
				edges[c] = 0;

		}
	}

	hysteresis(edges, rows, cols, s);

}

void hysteresis(Uint8 *edges, size_t rows, size_t cols, Stack_Tint* s){

	while (!stackTint_is_empty(s)){
		TupleInt e = stackTint_pop(s);
		size_t i = e.x;
		size_t j = e.y;

		if (i-1 >= 0){
			__hysteresis(edges, rows, cols, i-1, j); //N
			if (j-1 >= 0)
				__hysteresis(edges, rows, cols, i-1, j-1); //NW
			if (j+1 < cols)
				__hysteresis(edges, rows, cols, i-1, j+1); //NE
		}

		if (j-1 >=0)
			__hysteresis(edges, rows, cols, i, j-1); //W
		if (j+1 < cols)
			__hysteresis(edges, rows, cols, i, j+1); //E
		
		if (i+1 < rows){
			__hysteresis(edges, rows, cols, i+1, j); //S
			if (j-1 >= 0)
				__hysteresis(edges, rows ,cols, i+1, j-1); //SW
			if (j+1 < cols)
				__hysteresis(edges, rows, cols, i+1, j+1); //SE
		}

	}
}

void __hysteresis(Uint8 *edges, size_t rows, size_t cols, size_t i, size_t j){

	if (edges[i*cols+j] != WEAK)
		return;

	edges[i * cols + j] = STRONG;

	if (i-1 >= 0){
			__hysteresis(edges, rows, cols, i-1, j); //N
			if (j-1 >= 0)
				__hysteresis(edges, rows, cols, i-1, j-1); //NW
			if (j+1 < cols)
				__hysteresis(edges, rows, cols, i-1, j+1); //NE
		}

		if (j-1 >=0)
			__hysteresis(edges, rows, cols, i, j-1); //W
		if (j+1 < cols)
			__hysteresis(edges, rows, cols, i, j+1); //E
		
		if (i+1 < rows){
			__hysteresis(edges, rows, cols, i+1, j); //S
			if (j-1 >= 0)
				__hysteresis(edges, rows ,cols, i+1, j-1); //SW
			if (j+1 < cols)
				__hysteresis(edges, rows, cols, i+1, j+1); //SE
		}

}

void clean(Uint8 *edges, size_t size){

	for (size_t i = 0; i < size; i++){
		if (edges[i] == WEAK)
			edges[i] = 0;
	}

}

CannyRes canny(SDL_Surface *image){

	Uint8 *edges = malloc(sizeof(Uint8) * image->w * image->h);
    Uint8 *angles = malloc(sizeof(Uint8) * image->w * image->h);
	sobel_c(image, edges, angles);

	Uint8 *maxima = calloc(image->w * image->h, sizeof(Uint8));
    non_maxima_suppr(edges, angles, image->h, image->w, maxima);
	free(edges);
	//free(angles);   
	float threshold = otsu_threshold(image);
	double_thresholding(maxima, image->h, image->w, 0.5, threshold);
	clean(maxima,image->w*image->h);
	apply_convolution(image, maxima, (size_t)image->h, (size_t)image->w);
	CannyRes res = {maxima, angles};
	return res;
	//free(maxima);

}
