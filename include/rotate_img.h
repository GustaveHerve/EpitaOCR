#include <SDL2/SDL.h>
#include "geometry.h"
#include "matrix.h"
#include "pixel.h"

#ifndef ROTATE_H
#define ROTATE_H

void rotate_img(SDL_Surface *img, double angle);
void rotate_img90(SDL_Surface *img, double angle);
double* get_hMatrix(double* old, double* n);
SDL_Surface* homographic_Transform(SDL_Surface *img, Square corners);
void homographicT(SDL_Surface *img, Square corners);

#endif
