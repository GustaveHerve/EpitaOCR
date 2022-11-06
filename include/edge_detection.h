#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <SDL2/SDL.h>
#include "utils.h"

void sobel_c(SDL_Surface* surf, int* edges, Uint8* angles);
void sobel(SDL_Surface* surf);
void gradient(int r1[], int r2[], int edges[], Uint8 angles[], size_t rows, size_t cols);
void non_maxima_suppr(int edges[], Uint8 angles[], size_t rows, size_t cols, int res[]);
CannyRes canny(SDL_Surface *image);
void double_thresholding(int *edges, size_t rows, size_t cols, float lowRatio, float highRatio); 
void hysteresis(int *edges, size_t rows, size_t cols, Stack_Tint* s);
void __hysteresis(int *edges, size_t rows, size_t cols, size_t i, size_t j);

#endif
