#ifndef MATRIX_H
#define MATRIX_H

#include <SDL2/SDL.h>

void print_matrix(char s[], double m[], size_t rows, size_t cols);
void transpose(double m[], size_t rows, size_t cols, double r[]);
void add(double m1[], double m2[], size_t rows, size_t cols, double r[]);
void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[]);
void convolution(SDL_Surface *image, double ker[], size_t rows, 
		size_t cols, int r[], int sym);

void blur_convolution(SDL_Surface *image, double ker[], int rows, 
		int cols, int r[]);

int* get_cofactor(int* mat, int*res, int p, int q, int dim);
int get_determinant(int* mat, int dim);
int* get_adjoint(int* mat, int* adjoint, int dim);
int* inverseMat(int* mat, int* inverse, int dim);
#endif
