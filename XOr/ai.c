#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// define the number of each type of nodes
#define nInputs 2
#define nOutputs 1
#define nHiddenNodes 3
#define filename "Brain"

void InputValues(char* file, double hWeights[nInputs][nHiddenNodes],
    double oWeights[nHiddenNodes][nOutputs], double oBias[nOutputs])
{
    // this function is the beggining of the NN, it intputs the starting values
    // to the weights and output bias

 FILE * fp;
 fp = fopen(filename, "r");
 if (fp)
 {   // Input the previous values of weights and bias if they exist

     double myvariable;
     for (int i = 0; i < nInputs; i++)
         for (int j = 0; j < nHiddenNodes; j++)
         {
            fscanf(fp,"%lf",&myvariable);
            hWeights[i][j] += myvariable;
         }

     for (int i = 0; i < nHiddenNodes; i++)
         for (int j = 0; j < nOutputs; j++)
         {
            fscanf(fp,"%lf",&myvariable);
            oWeights[i][j] += myvariable;
         }
     for (int j = 0; j < nOutputs; j++)
         {
            fscanf(fp,"%lf",&myvariable);
            oBias[j] += myvariable;
         }

     fclose(fp);
 }
 else
 { // If there is no "Brain" file that means that it's the first time that
        // the NN if being launched, therefore we input random values.

     for (int i = 0; i < nInputs; i++)
         for (int j = 0; j < nHiddenNodes; j++)
             hWeights[i][j] = initWeights(); //Init Weights with random values

     for (int i = 0; i < nHiddenNodes; i++)
         for (int j = 0; j < nOutputs; j++)
             oWeights[i][j] = initWeights(); //Init Weights with random values

     for (int j = 0; j < nOutputs; j++)
         oBias[j] = initWeights(); //Init Output Layer Bias with random values

 }
}

void OutputValues(char* file, double hWeights[nInputs][nHiddenNodes],
      double oWeights[nHiddenNodes][nOutputs], double oBias[nOutputs])
{
    // This function "saves" the values of weights and bias in a file so that
    // the NN will keep his "knowledge" or "memory"
    // used after the training section

     FILE * fp;

     fp = fopen(filename, "w+");

     for(int j = 0; j < nInputs; j++)
        for(int k = 0; k < nHiddenNodes; k++)
            fprintf(fp, "%f\n", hWeights[k][j]);

     for(int j = 0; j < nHiddenNodes; j++)
        for(int k = 0; k < nOutputs; k++)
            fprintf(fp, "%f\n", oWeights[k][j]);

     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%f\n",oBias[j]);

     fclose(fp);

}


#define trainSets 4

int main(int argc, char **argv)
{
    int multiplier = 1;

    if (argc == 2)
        multiplier = argv[1][0] - '0';

    if (argc > 2)
    {
        fprintf(stderr, "To many arguments \n");
        return 1;
    }

	double lr = 0.1f; //Learning Rate

	double hLayer[nHiddenNodes];
	double oLayer[nOutputs];

	double hBias[nHiddenNodes];
	double oBias[nOutputs];

    int numberOfTimes = 5000 * multiplier;
    int memo_val = 0;

    double memory[numberOfTimes*trainSets];

	double (*hWeights)[nHiddenNodes] =
        malloc(sizeof(double[nInputs][nHiddenNodes]));

	double (*oWeights)[nOutputs] =
        malloc(sizeof(double[nHiddenNodes][nOutputs]));

	double trainInput[trainSets][nInputs]=  {{0.0f, 0.0f},{0.0f,1.0f},
        {1.0f,0.0f},{1.0f,1.0f}};

	double trainOutputs[trainSets][nOutputs] = {{0.0f},{1.0f},
        {1.0f},{0.0f}}; //results


    InputValues(filename, hWeights, oWeights, oBias);
	int trainingSetOrder[] = {0,1,2,3}; // There are 4 Training sets

	//TRAINING PART


	// Num of Epoch is the num of times it goes through the dataset
	for (int epoch = 0; epoch < numberOfTimes; epoch++)
	{
		shuffle(trainingSetOrder, trainSets);

		for(int x = 0; x < trainSets; x++)
		{
            memo_val++;
			int i = trainingSetOrder[x];

			// "Forward Path"
			// Compute the Input Layer Activation >>
			// Here, Sigmoid is the activation function

			for (int j = 0; j < nHiddenNodes; j++)
			{
				double act = hBias[j];

				for (int k = 0; k < nInputs; k++)
					act += trainInput[i][k] * hWeights[k][j];

				hLayer[j] = sigmoid(act); //Input mult with Bias
			}

			// Compute the Output Layer Activation >>

			for (int j = 0; j < nOutputs; j++)
			{
				double act = hBias[j];

				for (int k = 0; k < nHiddenNodes; k++)
					act += hLayer[k] * oWeights[k][j];

				oLayer[j] = sigmoid(act); //Input mult with Bias
			}


			printf("Input : (%g,%g)  Output:%g    Expected Output: %g\n",
                    trainInput[i][0], trainInput[i][1],
                    (oLayer[0]), trainOutputs[i][0]);


			// Backprop => Update the weights in function of the errors

			double dOutput[nOutputs]; // Delta Outputs

			for(int j = 0; j < nOutputs; j++)
			{
				double err = (trainOutputs[i][j] - oLayer[j]);
                memory[memo_val] = err;
				dOutput[j] = err * dSigmoid(oLayer[j]);
			}

			double dHidden[nHiddenNodes]; // Detla of Hidden layer here


			for(int j = 0; j < nHiddenNodes; j++)
			{
				double err = 0.0f;

				for(int k = 0; k < nOutputs; k++)
					err += dOutput[k] * oWeights[j][k];

				dHidden[j] = err * dSigmoid(hLayer[j]);
			}


			//Apply changes in outp Weights
			for(int j = 0; j < nOutputs; j++)
			{
				oBias[j] += dOutput[j] * lr;

				for(int k = 0; k < nHiddenNodes; k++)
					oWeights[k][j] += hLayer[k] * dOutput[j] * lr;
			}

			// For input weights
			for(int j = 0; j < nHiddenNodes; j++)
			{
				hBias[j] += dHidden[j] * lr;

				for(int k = 0; k < nHiddenNodes; k++)
					hWeights[k][j] += trainInput[i][k] * dHidden[j] * lr;
			}


		}
	}

    // In this part we print the precision of the NN and his weights

    int res = (int) (Precision(memo_val,memory) * 100);

    printf("\n The Precision is : %d",res);
    printf("%c \n\n", 37);

    // Here we print the values of the weights

    PrintValues(nInputs,nHiddenNodes,hWeights,"Hidden Weights");
    PrintValues(nHiddenNodes, nOutputs,oWeights, "Output Weights");

    // Finally we save the values of Weights and output bias in a file.

    OutputValues(filename, hWeights, oWeights, oBias);
    
    free(hWeights);

	return 0;


}



