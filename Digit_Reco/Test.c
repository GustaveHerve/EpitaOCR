#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/image_loading.h"
#include "include/pixel.h"
#include <dirent.h>

// define the number of each type of nodes
#define nInputs 784
#define nOutputs 10
#define nHiddenNodes 10
#define filename "Brain"

void InputValues(char* file, double hWeights[nInputs][nHiddenNodes],
    double oWeights[nHiddenNodes][nOutputs], double oBias[nOutputs],
    double hBias[], double hLayer[], double oLayer[])
{
    // this function is the beggining of the NN, it intputs the starting values
    // to the weights and output bias

 FILE * fp;
 fp = fopen(filename, "r");
 if (fp)
 {   // Input the previous values of weights and bias if they exist

     double myvariable;

    // Weights first

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

    // Bias second

     for (int j = 0; j < nOutputs; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        oBias[j] += myvariable;
     }

     // This part i'm not sure !
     for (int j = 0; j < nHiddenNodes; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        hBias[j] += myvariable;
     }

     // Layers

     for (int j = 0; j < nHiddenNodes; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        hLayer[j] += myvariable;
     }

     for (int j = 0; j < nOutputs; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        oLayer[j] += myvariable;
     }

     // end

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
                                             //
     for (int j = 0; j < nOutputs; j++)
         oBias[j] = initWeights(); //Init Output Layer Bias with random values

 }
}

void OutputValues(char* file, double hWeights[nInputs][nHiddenNodes],
      double oWeights[nHiddenNodes][nOutputs], double oBias[nOutputs],
      double hBias[], double hLayer[], double oLayer[])
{
    // This function "saves" the values of weights and bias in a file so that
    // the NN will keep his "knowledge" or "memory"
    // used after the training section

     FILE * fp;

     fp = fopen(filename, "w+");

     for(int j = 0; j < nInputs; j++)
        for(int k = 0; k < nHiddenNodes; k++)
            fprintf(fp, "%f\n", hWeights[j][k]);

     for(int j = 0; j < nHiddenNodes; j++)
        for(int k = 0; k < nOutputs; k++)
            fprintf(fp, "%f\n", oWeights[j][k]);

     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%f\n",oBias[j]);

     for (int j = 0; j < nHiddenNodes; j++)
          fprintf(fp,"%lf\n",hBias[j]);

     for (int j = 0; j < nHiddenNodes; j++)
          fprintf(fp,"%lf\n",hLayer[j]);

     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%lf\n",oLayer[j]);

     fclose(fp);

}


int Train(int argc, char *argv)
{

    if (argc > 2)
    {
        fprintf(stderr, "To many arguments \n");
        return 1;
    }
    argc--;
	double lr = 0.1f; //Learning Rate

	double (*hLayer) = malloc(sizeof(double[nHiddenNodes])); 
	double (*oLayer) = malloc(sizeof(double[nOutputs]));

	double (*hBias) = malloc(sizeof(double[nHiddenNodes])); 
	double (*oBias) = malloc(sizeof(double[nOutputs])); 

    int numberOfTimes = 5000;
    if (argc)
        numberOfTimes = 1;

    int memo_val = 0;
    int Result;

    int trainSets = 10;
    if (argc)
        trainSets = 1;

    double memory[numberOfTimes*trainSets];

	double (*hWeights)[nHiddenNodes] =
        malloc(sizeof(double[nInputs][nHiddenNodes]));

	double (*oWeights)[nOutputs] =
        malloc(sizeof(double[nHiddenNodes][nOutputs]));

	double (*trainInput)[nInputs] =
        malloc(sizeof(double[trainSets][nInputs]));


	double trainOutputs[10][nOutputs] =
    {
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

         {0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f},

         {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

         {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f},

         {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f},

         {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 1.0f},


    }; // Excpected results
     init_sdl();
     SDL_Surface* image;
     // Training sets
     if (argc)
     {
        char fn[50];
        strcpy( fn, "Train/SET5/" );
        char* s = strcat(fn,argv);
        printf("%s \n",s);
        image = load_image(s);
        parcours_pixel(image,trainInput[0]);
     }

     else
     {

     image = load_image("Train/SET5/00.png");
     parcours_pixel(image,trainInput[0]);
     image = load_image("Train/SET5/01.png");
     parcours_pixel(image,trainInput[1]);
     image = load_image("Train/SET5/02.png");
     parcours_pixel(image,trainInput[2]);
     image = load_image("Train/SET5/03.png");
     parcours_pixel(image,trainInput[3]);
     image = load_image("Train/SET5/04.png");
     parcours_pixel(image,trainInput[4]);
     image = load_image("Train/SET5/05.png");
     parcours_pixel(image,trainInput[5]);
     image = load_image("Train/SET5/06.png");
     parcours_pixel(image,trainInput[6]);
     image = load_image("Train/SET5/07.png");
     parcours_pixel(image,trainInput[7]);
     image = load_image("Train/SET5/08.png");
     parcours_pixel(image,trainInput[8]);
     image = load_image("Train/SET5/09.png");
     parcours_pixel(image,trainInput[9]);
     /*
     image = load_image("Train/SET4/00.png");
     parcours_pixel(image,trainInput[0]);
     image = load_image("Train/SET4/01.png");
     parcours_pixel(image,trainInput[1]);
     image = load_image("Train/SET5/02.png");
     parcours_pixel(image,trainInput[2]);
     image = load_image("Train/SET4/03.png");
     parcours_pixel(image,trainInput[3]);
     image = load_image("Train/SET4/04.png");
     parcours_pixel(image,trainInput[4]);
     image = load_image("Train/SET4/05.png");
     parcours_pixel(image,trainInput[5]);
     image = load_image("Train/SET4/06.png");
     parcours_pixel(image,trainInput[6]);
     image = load_image("Train/SET4/07.png");
     parcours_pixel(image,trainInput[7]);
     image = load_image("Train/SET4/08.png");
     parcours_pixel(image,trainInput[8]);
     image = load_image("Train/SET4/09.png");
     parcours_pixel(image,trainInput[9]);
     */
    }

    InputValues(filename, hWeights, oWeights, oBias, hBias, hLayer, oLayer);
	int trainingSetOrder[] = {0,1,2,3,4,5,6,7,8,9};

	//TRAINING TIME


	// Num of Epoch is the num of times it goes through the dataset
	for (int epoch = 0; epoch < numberOfTimes; epoch++)
	{
        if (!argc)
		    shuffle(trainingSetOrder, trainSets);

		for(int x = 0; x < trainSets; x++)
		{
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

                oLayer[j] = sigmoid(act);

			}

            Result = GetMax(nOutputs, oLayer);
            memory[memo_val] = (Result == i); //modify
            memo_val++;
            
            if (!argc)
			    printf("Input : %d  Output: %d \n",
                    i,Result);
            else
                printf("Image is a %d \n", Result);
            

			// Backprop => Update the weights in function of the errors
            if (!argc)
            {
			    double dOutput[nOutputs]; // Delta Outputs

			    for(int j = 0; j < nOutputs; j++)
			    {
                    double err = (trainOutputs[i][j] - oLayer[j]);
				    dOutput[j] = err * (dSigmoid(oLayer[j]));
			    }

			    double dHidden[nHiddenNodes]; // Detla of Hidden layer here


			    for(int j = 0; j < nHiddenNodes; j++)
			    {
				    double err = 0.0f;

				    for(int k = 0; k < nOutputs; k++)
					    err += dOutput[k] * oWeights[j][k];

				    dHidden[j] = (dSigmoid(hLayer[j])) * err ;
			    }


			    //Apply changes in outp Weights
			    for(int j = 0; j < nOutputs; j++)
			    {
				    oBias[j] += (dOutput[j] * lr);

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
	   }

    // In this part we print the precision of the NN and his weights

    int res = (int) (Precision(memo_val,memory) * 100);
    /*
    if (!argc)
    {
        printf("\n The Precision is : %d",res);
        printf("%c \n\n", 37);
    }
    */

    // Here we print the values of the weights
    //if (argc)
        //PrintValues(nInputs,nHiddenNodes,hWeights,"Hidden Weights");
    //PrintValues(nHiddenNodes, nOutputs,oWeights, "Output Weights");

    // Finally we save the values of Weights and output bias in a file.
    
    if (!argc)
        OutputValues(filename, hWeights, oWeights, oBias, hBias, hLayer, oLayer);

    free(hWeights);
    free(trainInput);
    free(oLayer);
    free(oWeights);


	return 0;


}

int main(int argc, char **argv)
{

    //Train(1,NULL);
    DIR *d;
    struct dirent *dir;
    d = opendir(argv[1]);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, "png") != NULL)
            {   
                printf("%s \n", dir->d_name);
                Train(2,dir->d_name);
            }
        }
        closedir(d);
    }

  return 0;

}




