#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include "utils.h"

void greyscale(SDL_Surface *image);
void threshold(SDL_Surface* image, float threshold);
void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, size_t cols);
void apply_convolution_int(SDL_Surface *image, int r[], size_t rows, size_t cols);
void gradient(int r1[], int r2[], Uint8 edges[], Uint8 angles[], size_t rows, size_t cols);
void non_maxima_suppr(Uint8 edges[], Uint8 angles[], size_t rows, size_t cols, Uint8 res[]);
void double_thresholding(Uint8 *edges, size_t rows, size_t cols, float lowRatio, float highRatio); 
void otsu(SDL_Surface* image);
void hough_init(int res[], int rows, int cols);
void hough_lines(SDL_Surface* image, int angleNb, int step, int res[]);
int hough_filter(int input[], int rows, int cols, int threshold, Line res[]);
TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, int step, Line hor[], Line ver[]);
int hough_filter_debug(int input[], int rows, int cols, int thresold, int step, Line res[]);
TupleInt hough_filter_localdebug(int input[], int rows, int cols, int threshold, Line hor[], Line res[]);

#endif
