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
  return((double)rand()) / ((double)RAND_MAX); // Create random Weights between
					       // 0 and 1 to init.
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

int transform(double val) {
     if (val < 0.5f)
          return 0;
      return 1;
 }


double Precision(int num, double memo[num]){
    double res = 0;
    for (int i = 0; i < num; i++){
        double acc = memo[i];
        if (acc < 0){
            res += (double) transform((1+acc));
        }
        else{
            res += (double) transform((1-acc));
        }
    }

    return res/num;


}


void PrintValues(int rows, int cols, double mat[rows][cols], char * name){
    printf("Values of %s : \n [",name);
    for (int i= 0; i < rows; i++)
        for (int j = 0; j < cols; j++){
            if (i + 1 == rows && j+1 == cols)
                printf(" %f ",mat[i][j]);
            else    
                printf(" %f, ",mat[i][j]);
        }
    printf("] \n\n");  

}

