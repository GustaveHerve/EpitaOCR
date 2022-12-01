#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <SDL2/SDL.h>

float otsu_threshold(SDL_Surface* image);
void otsu(SDL_Surface* image);

void threshold(SDL_Surface* image, float threshold);
void adaptive_thresholding(SDL_Surface *image, int size, int c);

#endif
