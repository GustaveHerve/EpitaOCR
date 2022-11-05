#ifndef GRID_DETECTION_H
#define GRID_DETECTION_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "geometry.h"

void hough_init(int res[], int rows, int cols);
void hough_lines(SDL_Surface* image, int angleNb, int step, int res[]);
void hough_lines_gradient(SDL_Surface* image, int angleNb, Uint8 *edges, Uint8 *angles, int res[]);
int hough_filter(int input[], int rows, int cols, int threshold, Line res[]);
TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, int step, int tol, Line hor[], Line ver[]);
int* line_distances(Line* lines, int len, int* res);
int get_grid_lines(Line* lines, int len, int* dis, int tolerance, Line* res);
int get_grid(Line* lines, int len, int tolerance, Line* res);
int merge_lines(Line* lines, int len, int thresh, Line* res);

#endif
