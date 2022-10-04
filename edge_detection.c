#include <stdlib.h>
#include "include/matrix.h"
#include "include/image_processing.h"

void sobel_c(SDL_Surface* surf, Uint8* edges, Uint8* angles){

	int *r1 = malloc(sizeof(int) * surf->w * surf->h);
    int *r2 = malloc(sizeof(int) * surf->w * surf->h);

	double kerx[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
    double kery[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};  

	convolution(surf, kerx, 3, 3, r1, 0);
    convolution(surf, kery, 3, 3, r2, 0);

	gradient(r1, r2, edges, angles, surf->h, surf->w);

	free(r1);
    free(r2);

}

void sobel(SDL_Surface* surf){

	Uint8 *edges = malloc(sizeof(Uint8) * surf->w * surf->h);
	Uint8 *angles = malloc(sizeof(Uint8) * surf->w * surf->h);
	sobel_c (surf, edges, angles);
    apply_convolution(surf, edges, (size_t)surf->h, (size_t)surf->w);

}
