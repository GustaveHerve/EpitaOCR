#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int max2(int a, int b);
int max3(int a, int b, int c);

typedef struct Histo{
	int values[256];
} Histo;

typedef struct Line{
	int rho;
	float theta;
} Line;

typedef struct TupleInt{
	int x;
	int y;
} TupleInt;

void histo_init(Histo* hist);
void histo_compute(SDL_Surface* image, Histo* hist);

float array_sum(float arr[], int begin, int end);
int array_min_index(float arr[], size_t len);

int polar_intersection(TupleInt *res, Line line1, Line line2);

#endif
