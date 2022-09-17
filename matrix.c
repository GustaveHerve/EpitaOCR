#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "include/pixel.h"

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
	printf("%s = \n", s);
	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){
			printf("%4g", m[i * cols + j]); 
		}

		if (i != rows)
			printf("\n");
	}
}

void transpose(double m[], size_t rows, size_t cols, double r[])
{
	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){
			r[j * rows + i] = m[i * cols + j];
		}
	}
}

void add(double m1[], double m2[], size_t rows, size_t cols, double r[])
{
	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){
			r[i * cols + j] = m1[i * cols + j] + m2[i * cols +j];
		}
	}
}

void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[])
{
	for (size_t i = 0; i < r1; i++){
		for (size_t j = 0; j < c2; j++){
			double sum = 0;
			for (size_t k = 0; k < c1; k++){
				sum += m1[i * c1 + k] * m2[k * c2 + j];
			}
			r[i * r1 + j] = sum;
		}
	}
}

void convolution(SDL_Surface *image, double ker[], int rows, 
		int cols, Uint8 r[], int sym){

	int width = image->w;
	int height = image->h;
	int len = height * width;

	double kert[rows * cols];
	if (!sym){
		transpose(ker, rows, cols, kert);
	}

	Uint32 *pixels = malloc(sizeof(Uint32) * len);
	get_pixel_array(image, pixels);

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){

			int acc = 0;
			int valid = 1;
			for (int k = -1; k < rows -1 && valid; k++){
				for (int l = -1; l < cols - 1 && valid ; l++){
					if (i+k >= 0 && j+l >= 0 && i+k <height && j+l < width){
					    int res = 0;
						Uint8 value = 0;
						SDL_GetRGB(pixels[(i+k)*width+(j+l)], image->format, &value, &value, &value);
						
						if (sym){
							res =
								ker[(k+1) * cols + l+1] * value;
						}
						else{
							res =
								kert[(k+1) * cols + l+1] * value;
						}
						
						acc += res;
					}
					else
						valid = 0;
				}
			}
			if (!valid)
				acc = 0;
			else if (acc > 255)
				acc = 255;
			else if (acc < 0)
				acc = 0;
			r[i*width + j] = (Uint8)acc;
		}
	}
}

void gradient(Uint8 r1[], Uint8 r2[], Uint8 res[], size_t rows, size_t cols){
	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){	
			double s1 = (double)r1[i*cols+j] * (double)r1[i*cols+j];
			double s2 = (double)r2[i*cols+j] * (double)r2[i*cols+j];
			double temp = sqrt(s1+s2);
			if (temp > 255)
				temp = 255;
			res[i*cols + j] = (Uint8)temp;
		}	
	}
}