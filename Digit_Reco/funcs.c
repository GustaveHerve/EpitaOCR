#include "funcs.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/image_loading.h"
#include "include/pixel.h"


double sigmoid(double x) // Sigmoid function used for the improvement bt part.
{
  return 1 / (1 + exp(-x));
}

double dSigmoid(double x) //Derivative of Sigmoid Func.
{
  return x * (1 - x);
}

double initWeights() // Random weights between -1 and 1.
{
    int rm = rand();
    if (rm % 2)
        return -((double)rand()) / ((double)RAND_MAX);
    return ((double)rand() / (double)RAND_MAX);
}

int NumberOfPixels(SDL_Surface* image){
    unsigned int width = image->w;
    unsigned int height = image->h;
    return width * height;
}

void parcours_pixel(SDL_Surface* image, double trainInput[])
{
    unsigned int width = image->w;
    unsigned int height = image->h;
    Uint32 *pixels = image->pixels;
    int acc = 0;
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++)
        {
            //Pixel intensity
            Uint8 val = 0;
            //Get pixel intensity and store it in val
            SDL_GetRGB(pixels[i*width + j], image->format, &val, &val, &val);
            //New val from a 0-255 int to a 0-1 int
            if (val == 255)
                val = 0;
            else
                val = 1;

            trainInput[acc] = val;
            acc++;

        }


}

void shuffle(int *arr, size_t t) // Shuffling data serves the purpose of
				 // reducing variance and making sure that
				 // models remain general and overfit less.
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

int transform(double val)  //Transforms an double to an int, mostly usefull for
                           //XOR NN
{
    if (val < 0.5f) // serves the purpose of printing "0" instead of 0.0032....
          return 0;

     if (val < 1.5f) // serves the purpose of printing "1" instead of 0.99....
         return 1;
     if (val < 2.5f)
         return 2;
     if (val < 3.5f)
         return 3;
     if (val < 4.5f)
         return 4;
     if (val < 5.5f)
         return 5;
     if (val < 6.5f)
         return 6;
     if (val < 7.5f)
         return 7;
     if (val < 8.5f)
         return 8;
     return 9;
 }


double Precision(int num, double memo[num])
{
    double res = 0;

    for (int i = 0; i < num; i++)
        res += memo[i];

    return res/num;

}


void PrintValues(int rows, int cols, double mat[rows][cols], char * name)
{
    printf("Values of %s : \n [",name);

    for (int i= 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if (i + 1 == rows && j+1 == cols)
                printf(" %f ",mat[i][j]);

            else
                printf(" %f, ",mat[i][j]);
        }

    printf("] \n\n");

}

int GetMax(int size, double arr[size])
{
    int max = 0;

    for (int i = 0; i < 10; i++)
    {
        //printf("%g \n", arr[i]);
        if (arr[i] > arr[max])
           max = i;
    }

    return max;
}



