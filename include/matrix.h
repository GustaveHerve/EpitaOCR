#ifndef MATRIX_H
#define MATRIX_H

#include <SDL2/SDL.h>

void print_matrix(char s[], double m[], size_t rows, size_t cols);
void transpose(double m[], size_t rows, size_t cols, double r[]);
void add(double m1[], double m2[], size_t rows, size_t cols, double r[]);
void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[]);
void convolution(SDL_Surface *image, double ker[], size_t rows, 
		size_t cols, int r[], int sym);

double* get_cofactor(double* mat, double*res, int p, int q, int dim);
double get_determinant(double* mat, int dim);
double* get_adjoint(double* mat, double* adjoint, int dim);
double* inverseMat(double* mat, double* inverse, int dim);
#endif
