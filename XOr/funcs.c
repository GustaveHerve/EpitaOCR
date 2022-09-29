#include "funcs.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double sigmoid(double x) // Sigmoid function used for the improvement bt part.
{
  return 1 / (1 + exp(-x));
}

double dSigmoid(double x) //Derivative of Sigmoid Func.
{
  return x * (1 - x);
}

double initWeights()
{
  return((double)rand()) / ((double)RAND_MAX); // Create random Weights between 0 and 1 to init.
}

void shuffle(int *arr, size_t t) // Shuffling data serves the purpose of reducing variance and making sure that models remain general and overfit less.
{
  if (t > 1)
  {
    for (size_t i = 0; i < t - 1; i++)
      {
        size_t j = i + rand() / (RAND_MAX / (t - i) + 1);
        int data = arr[j];
        arr[j] = arr[i];
        arr[i] = data;
      }
  }
}
