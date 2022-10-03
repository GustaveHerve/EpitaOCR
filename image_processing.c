#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "include/pixel.h"
#include "include/utils.h"

void greyscale(SDL_Surface *image){

	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			Uint32 newpixel = SDL_MapRGB(image->format, average, average,
					average);

			replace_pixel(image, j, i, newpixel);

		}
	}
}

//threshold: applies a fixed threshold treatment to a SDL_Surface
void threshold(SDL_Surface* image, float threshold){

	unsigned int width = image->w;
	unsigned int height = image->h;
	Uint8 t = threshold * 255;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){
			
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 res = 0;
			if (r >= t){
				res = 255;
			}

			Uint32 newpixel = SDL_MapRGB(image->format, res, res, res);
			replace_pixel(image, j, i, newpixel);
				

		}
	}
}

void threshold_value(SDL_Surface* image, int threshold){

	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 res = 0;
			if (r > threshold){
				res = 255;
			}

			Uint32 newpixel = SDL_MapRGB(image->format, res, res, res);
			replace_pixel(image, j, i, newpixel);
		}
	}
}

void otsu(SDL_Surface* image){

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
	threshold_value(image, thresh);


}

//apply_convolution: applies the result of a convolution (stored in r[]) to an SDL_Surface image 
//rows and cols determines r[]'s size
void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, size_t cols){
   
    for (size_t i = 0; i < rows; i++){
        for (size_t j= 0; j < cols; j++){
            Uint32 pixel = SDL_MapRGB(image->format, r[i*cols + j], r[i*cols + j],
					r[i*cols + j]);
            replace_pixel(image, j, i, pixel);
        }
    }
}

//apply_convolutio_int: same as previous but accept int array and clamp between 0 and 255 to avoid
//overflow
void apply_convolution_int(SDL_Surface *image, int r[], size_t rows, size_t cols){
   
    for (size_t i = 0; i < rows; i++){
        for (size_t j= 0; j < cols; j++){

			Uint8 value;
			if (r[i*cols+j] > 255)
				value = 255;
			else if (r[i*cols+j] < 0)
				value = 0;
			else
				value = r[i*cols+j];

            Uint32 pixel = SDL_MapRGB(image->format, value, value,
					value);
            replace_pixel(image, j, i, pixel);
        }
    }
}
/* gradient: computes edge and angle gradient values from two matrices of same size
*  stores the result in edges[] and angles[] (so it doesn't affect the image directly)
*/
void gradient(int r1[], int r2[], Uint8 edges[], Uint8 angles[], size_t rows, size_t cols){

	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){	
			int current = i*cols + j;
			double s1 = (double)r1[current] * (double)r1[current];
			double s2 = (double)r2[current] * (double)r2[current];
			double temp = sqrt(s1+s2);
			//double temp = fabs(r1[current]) + fabs(r2[current]);
			if (temp > 255)
				temp = 255;
			edges[current] = (Uint8)temp;

			float tan = atan2(r2[current], r1[current]);
			tan = fabs(tan);
			tan = tan * 180 / M_PI;
			Uint8 angle = 0;

			if (tan >= 0 && tan <= 22.5)
				angle = 0;
			else if (tan <= 67.5)
				angle = 45;
			else if (tan <= 112.5)
				angle = 90;
			else
				angle = 135;

			angles[current] = angle;
		}	
	}
}

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
					int maxima = max3(edges[c-1], edges[c], edges[c+1]);
					if (maxima != edges[c])
						res[c] = 0;
					else
						res[c] = edges[c];
				}
				break;

			case 45:
				if ((j+1 < (int)cols && i+1 < (int)rows) && (i-1 >= 0 && j-1 >= 0)){
					int maxima = max3(edges[(i-1)*cols+j-1], edges[c], edges[(i+1)*cols+j+1]);
					if (maxima != edges[c])
						res[c] = 0;
					else
						res[c] = edges[c];

				}
				break;

			case 90:
				if (i+1 < (int)cols && i-1 >= 0){
					int maxima = max3(edges[((i-1)*cols+j)], edges[c], edges[(i+j)*cols+j]);
					if (maxima != edges[c])
						res[c] = 0;
					else
						res[c] = edges[c];

				}
				break;

			case 135:
				if ((j-1 >= 0 && i+1 < (int)rows ) && (j+1 < (int)cols && i-1 >= 0)){
					int maxima = max3(edges[(i+1)*cols+j-1], edges[c], edges[(i-1)*cols+j+1]);
					if (maxima != edges[c])
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
		float lowRatio, float highRatio){

	int highValue = highRatio * 255;
	int lowValue = highValue * lowRatio;
	int weak = 100;
	int strong = 255;

	for (size_t i = 0; i < rows; i++){
		for (size_t j = 0; j < cols; j++){
			int c = i * cols + j;

			if (edges[c] >= highValue)
				edges[c] = strong;
			else if (edges[c] > lowValue)
				edges[c] = weak;
			else
				edges[c] = 0;

		}
	}
}

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
			Uint8 r=0, g=0, b=0;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r > 0)
			{
				for (int k = 0; k < angleNb; k+=step){
					
					float rad = k * M_PI / 180;
					float rho = j * cos(rad) + i * sin(rad);
					int rhoi = roundf(rho);
					if (rhoi < 0)
						break;
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
				if (jmax < 45){
					hor[horacc] = new;
					horacc++;
				}
				else{
					ver[veracc] = new;
					veracc++;
				}
			}
		}
	}

	TupleInt res;
	res.x = horacc;
	res.y = veracc;
	return res;
}

int hough_filter_debug(int input[], int rows, int cols, int threshold, int step, Line res[]){

	int acc = 0;

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
				res[acc] = new;
				acc++;
			}
		}
	}

	return acc;
}

TupleInt hough_filter_localdebug(int input[], int rows, int cols, int threshold, Line hor[], Line ver[]){

	int horacc = 0;
	int veracc = 0;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			if (input[i * cols + j] >= threshold){
				Line new;
				new.theta = j;
				new.rho = i;
				if (j > 45){
					hor[horacc] = new;
					horacc++;
				}
				else{
					ver[veracc] = new;
					veracc++;
				}
			}
		}
	}
	TupleInt res;
	res.x = horacc;
	res.y = veracc;
	return res;
}
