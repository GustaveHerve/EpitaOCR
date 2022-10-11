#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <SDL2/SDL.h>

void sobel_c(SDL_Surface* surf, Uint8* edges, Uint8* angles);
void sobel(SDL_Surface* surf);
void gradient(int r1[], int r2[], Uint8 edges[], Uint8 angles[], size_t rows, size_t cols);
void non_maxima_suppr(Uint8 edges[], Uint8 angles[], size_t rows, size_t cols, Uint8 res[]);
void canny(SDL_Surface *image);

#endif
