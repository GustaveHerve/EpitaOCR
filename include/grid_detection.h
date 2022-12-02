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
int merge_lines(Line* lines, int len, int thresh, Line* res);

int* line_distances(Line* lines, int len, int* res);
int get_grid_lines(Line* lines, int len, int* dis, Line* res, int *hough);
int get_grid_linesold(Line* lines, int len, int* dis, int tolerance, Line* res, int *hough);
int get_grid(Line* lines, int len, Line* res, int *hough);
int get_intersection_count(Segment s, Segment* segments, int len);

Segment *get_grid_seg(Segment* xseg, Segment* yseg, TupleInt lens);
Square get_blobs(Line* lines, int len, int width, int height);

#endif
