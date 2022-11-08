#ifndef GRID_EXTRACTION_H
#define GRID_EXTRACTION_H

#include "geometry.h"
#include <SDL2/SDL.h>

int get_squares(Line *x, Line *y, Square *res);
Square *get_squares_seg(Segment *grid);

void save_squares(Square *sq, int len, SDL_Surface *image);
void save_squares_seg(Square *sq, SDL_Surface *image, char* path);

#endif
