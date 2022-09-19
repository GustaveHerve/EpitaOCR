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

void otsunotworking(SDL_Surface* image){

	int n = image->w * image->h;
	//int t = 0, var_max = 0, sum = 0, sumB = 0;
	//int q1 = 0, q2 = 0, u1 = 0, u2 = 0;

	Histo hist;
	histo_init(&hist);
	histo_compute(image, &hist);

	float varinter[256];

	float proba[256];
	for (int i = 0; i < 256; i++){
		proba[i] = (float)hist.values[i] / n;
	}

	for (int j = 1; j < 256; j++){
		float p1 = array_sum(proba, 0, j+1);
	    float p2 = array_sum(proba, j+1, 256);

		float n1[j];
		for (int k = 0; k < j; k++){
			n1[k] = k;
		}

		float n2[256-j];
		for (int l = j; l < 256; l++){
			n2[l] = l;
		}

		float moy1 = 0;
		float sum1 = 0;
		for (int m = 0; m < j; m++){
			sum1 += n1[m] * proba[m];
		}
		moy1 = sum1 / p1;

		float moy2 = 0;
		float sum2 = 0;
		for (int o = j; o < 256; o++){
			sum2 += n2[o-j] * proba[o];
		}
		moy2 = sum2 / p2;

		float var1 = 0;
		for (int p = 0; p < j; p++){
			var1 += (n1[p] - moy1) * (n1[p] - moy1) * proba[p];
		}
		var1 = var1 / p1;

		float var2 = 0;
		for (int q = j; q < 256; q++){
			var2 += (n2[q-j] - moy2) * (n2[q-j] - moy2) * proba[q];
		}
		var2 = var2 / p2;

		varinter[j] = var1 + var2;

	}

	float indice = array_min_index(varinter, 256);
	float level = (indice-1) / 255;
	threshold(image, level);


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
		sum += i * hist.values[i];
	}

	for (int t = 0; t <= max_intensity; t++){
		q1 += hist.values[t];
		if (q1 == 0){
			continue;
		}
		q2 = n - q1;

		sumB += t * hist.values[t];
		u1 = sumB / q1;
		u2 = (sum - sumB) / q2;

		float var = q1 * q2 * ((u1 - u2) * (u1 - u2));

		if (var > var_max){
			thresh = t;
			var_max = var;
		}

	}
	threshold(image, thresh);

	

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

void non_maxima_suppr(Uint8 edges[], Uint8 angles[], size_t rows, size_t cols){
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
						edges[c] = 0;
				}
				break;

			case 45:
				if ((j+1 < (int)cols && i+1 < (int)rows) && (i-1 >= 0 && j-1 >= 0)){
					int maxima = max3(edges[(i-1)*cols+j-1], edges[c], edges[(i+1)*cols+j+1]);
					if (maxima != edges[c])
						edges[c] = 0;
				}
				break;

			case 90:
				if (i+1 < (int)cols && i-1 >= 0){
					int maxima = max3(edges[((i-1)*cols+j)], edges[c], edges[(i+j)*cols+j]);
					if (maxima != edges[c])
						edges[c] = 0;
				}
				break;

			case 135:
				if ((j-1 >= 0 && i+1 < (int)rows ) && (j+1 < (int)cols && i-1 >= 0)){
					int maxima = max3(edges[(i+1)*cols+j-1], edges[c], edges[(i-1)*cols+j+1]);
					if (maxima != edges[c])
						edges[c] = 0;
				}
				break;
			}

		}

	}
}
