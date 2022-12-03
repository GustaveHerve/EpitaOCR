#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "geometry.h"

void greyscale(SDL_Surface *image);
void blur(SDL_Surface *image, int kersize);
void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, size_t cols);
void apply_convolution_int(SDL_Surface *image, int r[], size_t rows, size_t cols);
void invert(SDL_Surface *image);
void gauss_blur(SDL_Surface* img, int size, double sigma);
void gauss_ker(int size, double sigma, double ker[]);

#endif
