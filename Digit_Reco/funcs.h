#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef FUNCS_H
#define FUNCS_H

double sigmoid(double x);
double dSigmoid(double x);
void shuffle(int *arr, size_t t);
double initWeights();
void PrintValues(int rows, int cols, double mat[rows][cols], char * name);
double Precision(int num, double memo[num]);
int transform(double val);
int NumberOfPixels(SDL_Surface* image);
void parcours_pixel(SDL_Surface* image, double *trainInput);
int GetMax(int size, double arr[size]);

#endif
