#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int max2(int a, int b);
int max3(int a, int b, int c);

typedef struct Histo{
	int values[256];
} Histo;

void histo_init(Histo* hist);
void histo_compute(SDL_Surface* image, Histo* hist);

float array_sum(float arr[], int begin, int end);
int array_min_index(float arr[], size_t len);

#endif
