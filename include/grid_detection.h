#ifndef GRID_DETECTION_H
#define GRID_DETECTION_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "geometry.h"

void hough_init(int res[], int rows, int cols);
void hough_lines(SDL_Surface* image, int angleNb, int step, int res[]);
int hough_filter(int input[], int rows, int cols, int threshold, Line res[]);
TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, int step, Line hor[], Line ver[]);

#endif
