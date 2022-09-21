#include <stdlib.h>
#include <SDL2/SDL.h>
#include "include/utils.h"
#include "include/pixel.h"

int max2(int a, int b){

    if (a < b)
        return b;
    return a;
}

int max3(int a, int b, int c){

    int temp = max2(a,b);
    if (temp < c)
        return c;
    return temp;
}

void histo_init(Histo* hist){

	for (unsigned int i = 0; i < 256; i++){
		hist->values[i] = 0;
	}
}

void histo_compute(SDL_Surface* image, Histo* hist){

	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++){
		for (unsigned int j = 0; j < width; j++){

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;

			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			hist->values[r] += 1;

		}
	}
}

float array_sum(float arr[], int begin, int end){

	float res = 0;
	for (int i = begin; i < end; i++){
		res += arr[i];
	}

	return res;
}

int array_min_index(float arr[], size_t len){
	size_t min = 0;
	for (size_t i = 1; i < len; i++){
		
		if (arr[i] < arr[min])
			min = i;
	}

	return (int) min;
}

