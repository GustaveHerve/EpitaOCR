#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "geometry.h"

void greyscale(SDL_Surface *image);
void blur(SDL_Surface *image, int kersize);
void threshold(SDL_Surface* image, float threshold);
void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, size_t cols);
void apply_convolution_int(SDL_Surface *image, int r[], size_t rows, size_t cols);
void double_thresholding(Uint8 *edges, size_t rows, size_t cols, float lowRatio, float highRatio); 
void otsu(SDL_Surface* image);


#endif
