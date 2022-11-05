#include <stdlib.h>
#ifndef FUNCS_H
#define FUNCS_H

double sigmoid(double x);
double dSigmoid(double x);
void shuffle(int *arr, size_t t);
double initWeights();
void PrintValues(int rows, int cols, double mat[rows][cols], char * name);
double Precision(int num, double memo[num]);
int transform(double val);

#endif
