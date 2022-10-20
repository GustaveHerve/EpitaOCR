#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Neural Network with 1 layer > XOR

// Define the nn
// define is cool because it is flexible
#define nInputs 2
#define nOutputs 1
#define nHiddenNodes 2
#define trainSets 4

int main()
{
	double lr = 0.1f; //Learning Rate
	/*
	 *
	 *	I Basically Define every elements of the layers here
	 *
	 *
	 */
	double hLayer[nHiddenNodes];
	double oLayer[nOutputs];
	
	double hBias[nHiddenNodes];
	double oBias[nOutputs];
    
    int numberOfTimes = 10000;
    int memo_val = 0;

    double memory[numberOfTimes*trainSets];

    int BrainExists = 0;

	double hWeights[nInputs][nHiddenNodes];
	double oWeights[nHiddenNodes][nOutputs];
	const double trainInput[trainSets][nInputs] = {{0.0f, 0.0f},{0.0f,1.0f}, 
        {1.0f,0.0f},{1.0f,1.0f}};
	double trainOutputs[trainSets][nOutputs] = {{0.0f},{1.0f},
        {1.0f},{0.0f}}; //results


/*
 *
 */

	FILE * fp;
	fp = fopen("Brain", "r");
	if (fp){
        BrainExists = 1;
		double myvariable;
		for (int i = 0; i < nInputs; i++)
                for (int j = 0; j < nHiddenNodes; j++)
                {	
			fscanf(fp,"%lf",&myvariable);
      			printf("%s,%f \n","In hWeights : \n",myvariable);
                        hWeights[i][j] += myvariable;
                }

		for (int i = 0; i < nHiddenNodes; i++)
		for (int j = 0; j < nOutputs; j++)
                {
			fscanf(fp,"%lf",&myvariable);
      			printf("%s,%f \n","In oWeights : \n",myvariable);
                        oWeights[i][j] += myvariable;
                }
		for (int j = 0; j < nOutputs; j++)
        	{
			fscanf(fp,"%lf",&myvariable);
                        printf("%s, %f \n"," In oBias : \n",myvariable);
                	oBias[j] += myvariable;
        	}
    }
/*
 * We enter this part if the file "Brain" doesn't exist, this emplies that it's the first time that the XOR NN is executed
 * I define all the weights with random values in order to start to backprop 
 * For this reason : https://machinelearningmastery.com/why-initialize-a-neural-network-with-random-weights/ 
 */

	else {

	for (int i = 0; i < nInputs; i++)
		for (int j = 0; j < nHiddenNodes; j++)
		{
			hWeights[i][j] = initWeights(); //Init Weights with random values
		}

	for (int i = 0; i < nHiddenNodes; i++)
		for (int j = 0; j < nOutputs; j++)
		{
			oWeights[i][j] = initWeights(); //Init Weights with random values
		}
	
	for (int j = 0; j < nOutputs; j++)
	{
		oBias[j] = initWeights(); //Init Output Layer Bias with random values
	}
	}
	int trainingSetOrder[] = {0,1,2,3}; // There are 4 Training sets
		
	//TRAINING TIME
	
	// From a bit of the super YT Video NN
	//
	//
	//
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
				{
					act += trainInput[i][k] * hWeights[k][j];
				}
				hLayer[j] = sigmoid(act); //Input mult with Bias
			}

			// Compute the Output Layer Activation >>

			for (int j = 0; j < nOutputs; j++)
			{
				double act = hBias[j];
				for (int k = 0; k < nHiddenNodes; k++)
				{
					act += hLayer[k] * oWeights[k][j];
				}
				oLayer[j] = sigmoid(act); //Input mult with Bias
			}


			printf("Input : (%g,%g)  Output:%g    Expected Output: %g\n",
                    trainInput[i][0], trainInput[i][1],
                    oLayer[0], trainOutputs[i][0]);
			// Magic print to get the results :D


			// Backprop => Update the weights in function of the errors
			//
			//
			// I don't really understand all the maths behind that, W.I.P
			// Spending my life on wikipedia baby UwU
			//
			//
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
				{
					err += dOutput[k] * oWeights[j][k];
				}
				dHidden[j] = err * dSigmoid(hLayer[j]);
			}
			

			//Apply changes in outp Weights
			for(int j = 0; j < nOutputs; j++)
			{
				oBias[j] += dOutput[j] * lr;
				for(int k = 0; k < nHiddenNodes; k++)
				{
					oWeights[k][j] += hLayer[k] * dOutput[j] * lr;
				}
			}
			
			// For input weights
			for(int j = 0; j < nHiddenNodes; j++)
			{
				hBias[j] += dHidden[j] * lr;
				for(int k = 0; k < nHiddenNodes; k++)
				{
					hWeights[k][j] += trainInput[i][k] * dHidden[j] * lr;
				}
			}


		}
	}
    double res = Precision(memo_val,memory) * 100;
    printf("\n The Precision is : %f",res);
    printf("%c \n\n", 37);
    
    PrintValues(nInputs,nHiddenNodes,hWeights,"Hidden Weights");

	// In this part we will save our values in the brain file
	
	fp = fopen("Brain", "w+");

	for(int j = 0; j < nInputs; j++)
           for(int k = 0; k < nHiddenNodes; k++)
              fprintf(fp, "%f\n", hWeights[k][j]);

	    for(int j = 0; j < nHiddenNodes; j++)
           for(int k = 0; k < nOutputs; k++)
              fprintf(fp, "%f\n", oWeights[k][j]);

	    for (int j = 0; j < nOutputs; j++)
              fprintf(fp,"%f\n",oBias[j]);
	

	fclose(fp);
	
	return 0;


}



