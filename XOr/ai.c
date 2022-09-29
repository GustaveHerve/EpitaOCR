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

	double hWeights[nInputs][nHiddenNodes];
	double oWeights[nHiddenNodes][nOutputs];

	double trainInput[trainSets][nInputs] = {{0.0f, 0.0f},{0.0f,1.0f}, //tests
																					{1.0f,0.0f},{1.0f,1.0f}};
	double trainOutputs[trainSets][nOutputs] = {{0.0f},{1.0f},{1.0f},{0.0f}}; //results


/*
 * Here, I define all the weights with random values in order to start to backprop 
 * For this reason : https://machinelearningmastery.com/why-initialize-a-neural-network-with-random-weights/ 
 */


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

	int trainingSetOrder[] = {0,1,2,3}; // There are 4 Training sets :D (Amogus)

	int numberOfTimes = 15000; // U dumb or you want a comment for that :/

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
			int i = trainingSetOrder[x];

			// "Forward Path"
			// Compute the Input Layer Activation >>
			// Here, Sigmoid is the activation function
			// https://machinelearningmastery.com/a-gentle-introduction-to-sigmoid-function/
			// Ce site est le fire OwO

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
			printf ("Input:(%g,%g)  Output:%g    Expected Output: %g\n",
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
}



