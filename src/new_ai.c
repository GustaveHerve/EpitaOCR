#include "../include/funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/image_loading.h"
#include "../include/pixel.h"
#include <dirent.h>
#include <locale.h>


// define the number of each type of nodes
#define nInputs 784
#define nOutputs 10
#define nHiddenNodes 175
#define filename "Brain"
#define IMAGES 300
#define STRING 100

void InputValues(char* file, double **hWeights,
    double **oWeights, double oBias[nOutputs],
    double hBias[], double hLayer[], double oLayer[])
{
    // this function is the beggining of the NN, it intputs the starting values
    // to the weights and output bias

 FILE * fp;
 fp = fopen(filename, "r");
 if (fp)
 {   // Input the previous values of weights and bias if they exist

     char *s = setlocale(LC_NUMERIC,"C");

     double myvariable;

    // Weights first

     for (int i = 0; i < nInputs; i++)
         for (int j = 0; j < nHiddenNodes; j++)
         {
            fscanf(fp,"%lf",&myvariable);
            hWeights[i][j] = myvariable;
         }

     for (int i = 0; i < nHiddenNodes; i++)
         for (int j = 0; j < nOutputs; j++)
         {
            fscanf(fp,"%lf",&myvariable);
            oWeights[i][j] = myvariable;
         }


     for (int j = 0; j < nOutputs; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        oBias[j] = myvariable;
     }

     // This part i'm not sure !
     for (int j = 0; j < nHiddenNodes; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        hBias[j] = myvariable;
     }

     // Layers
     /*
     for (int j = 0; j < nHiddenNodes; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        hLayer[j] = myvariable;
     }

     for (int j = 0; j < nOutputs; j++)
     {
        fscanf(fp,"%lf",&myvariable);
        oLayer[j] = myvariable;
     }
     */


     // end

     fclose(fp);
     setlocale(LC_NUMERIC,s);
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

     for (int j = 0; j < nHiddenNodes; j++)
         hBias[j] = initWeights();
     /*
     for (int j = 0; j < nOutputs; j++)
         oLayer[j] = initWeights();

     for (int j = 0; j < nHiddenNodes; j++)
         hLayer[j] = initWeights();
     */

 }
}

void OutputValues(char* file, double **hWeights,
      double **oWeights, double oBias[nOutputs],
      double hBias[], double hLayer[], double oLayer[])
{
    // This function "saves" the values of weights and bias in a file so that
    // the NN will keep his "knowledge" or "memory"
    // used after the training section
     FILE * fp;

     fp = fopen(filename, "w");

     for(int j = 0; j < nInputs; j++)
        for(int k = 0; k < nHiddenNodes; k++)
            fprintf(fp, "%lf\n", hWeights[j][k]);


     for(int j = 0; j < nHiddenNodes; j++)
        for(int k = 0; k < nOutputs; k++)
            fprintf(fp, "%lf\n", oWeights[j][k]);

     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%lf\n",oBias[j]);

     for (int j = 0; j < nHiddenNodes; j++)
          fprintf(fp,"%lf\n",hBias[j]);

    /*
     for (int j = 0; j < nHiddenNodes; j++)
          fprintf(fp,"%lf\n",hLayer[j]);


     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%lf\n",oLayer[j]);
     */

     fclose(fp);

}

void fill(char **arr, DIR *d, struct dirent *dir, char *name)
{
    int i = 0;
    char* fn = (char*)malloc(50 * sizeof(char));
    while ((dir = readdir(d)) != NULL && i < IMAGES)
    {
        if (strstr(dir->d_name, "png") != NULL
                || strstr(dir->d_name, "jpeg") != NULL)
        {
            strcpy( fn, name);
            char* s = strcat(fn,dir->d_name);
            printf("%s\n",s);
            strcpy(arr[i],s);
            i++;
        }
    }
    free(fn);
    closedir(d);
}

int ai(int argc, char *argv, char **im)
{
    /*
    printf("%s\n",im[0]);
    //printf("Test1\n");
    //printf("%s\n",im[1]);
    printf("Test2\n");
    //printf("%s\n",im[2]);
     printf("Test3\n");
    //printf("%s\n",im[3]);
     printf("Test4\n");
    //printf("%s\n",im[4]);
     printf("Test5\n");
    //printf("%s\n",im[5]);
     printf("Test6\n");
    //printf("%s\n",im[6]);
     printf("Test7\n");
    //printf("%s\n",im[7]);
     printf("Test8\n");
    //printf("%s\n",im[8]);
     printf("Test9\n");
    //printf("%s\n",im[9]);
    */

    if (argc > 2)
    {
        fprintf(stderr, "To many arguments \n");
        return 1;
    }
    argc--;
	double lr = 0.1f; //Learning Rate

    double* hLayer = (double*)malloc(nHiddenNodes * sizeof(double));
    double* oLayer = (double*)malloc(nOutputs * sizeof(double));
    double* hBias = (double*)malloc(nHiddenNodes * sizeof(double));
    double* oBias = (double*)malloc(nOutputs * sizeof(double));

    int numberOfTimes = 200;
    if (argc)
        numberOfTimes = 1;

    int memo_val = 0;
    int Result;

    int trainSets = 10;
    if (argc)
        trainSets = 1;

    //double* memory =
        //(double*)malloc((numberOfTimes*trainSets) * sizeof(double));

    double memory[numberOfTimes*trainSets];

    double** hWeights = (double**)malloc(nInputs * sizeof(double*));
    for (int i = 0; i < nInputs; i++)
        hWeights[i] = (double*)malloc(nHiddenNodes * sizeof(double));

    double** oWeights = (double**)malloc(nHiddenNodes * sizeof(double*));
    for (int i = 0; i < nHiddenNodes; i++)
        oWeights[i] = (double*)malloc(nOutputs * sizeof(double));

    double** trainInput = (double**)malloc(trainSets * sizeof(double*));
    for (int i = 0; i < trainSets; i++)
        trainInput[i] = (double*)malloc(nInputs * sizeof(double));

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
        image = load_image(argv);
        parcours_pixel(image,trainInput[0]);
        SDL_FreeSurface(image);
        //for (int i = 0; i < nInputs; i++)
            //printf("%lf",trainInput[0][i]);
     }

     else
     {

     image = load_image(im[0]);
     parcours_pixel(image,trainInput[0]);
     SDL_FreeSurface(image);

     SDL_Surface* image1;
     image1 = load_image(im[1]);
     parcours_pixel(image1,trainInput[1]);
     SDL_FreeSurface(image1);
    //for (int i = 0; i < nInputs; i++)
         //printf("%lf",trainInput[1][i]);
     SDL_Surface* image2;
     image2 = load_image(im[2]);
     parcours_pixel(image2,trainInput[2]);
     SDL_FreeSurface(image2);
     //for (int i = 0; i < nInputs; i++)
         //printf("%lf",trainInput[2][i]);
    SDL_Surface* image3;
     image3 = load_image(im[3]);
     parcours_pixel(image3,trainInput[3]);
    SDL_FreeSurface(image3);
    //for (int i = 0; i < nInputs; i++)
         //printf("%lf",trainInput[3][i]);
    SDL_Surface* image4;
     image4 = load_image(im[4]);
     parcours_pixel(image4,trainInput[4]);
     SDL_FreeSurface(image4);

     SDL_Surface* image5;
     image5 = load_image(im[5]);
     parcours_pixel(image5,trainInput[5]);
     SDL_FreeSurface(image5);

     SDL_Surface* image6;
     image6 = load_image(im[6]);
     parcours_pixel(image6,trainInput[6]);
     SDL_FreeSurface(image6);

     SDL_Surface* image7;
     image7 = load_image(im[7]);
     parcours_pixel(image7,trainInput[7]);
     SDL_FreeSurface(image7);

     SDL_Surface* image8;
     image8 = load_image(im[8]);
     parcours_pixel(image8,trainInput[8]);
     SDL_FreeSurface(image8);

     SDL_Surface* image9;
     image9 = load_image(im[9]);
     parcours_pixel(image9,trainInput[9]);
    //for (int i = 0; i < nInputs; i++)
         //printf("%lf",trainInput[9][i]);
     SDL_FreeSurface(image9);

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
            /*
            if (!argc)
			    printf("Input : %d  Output: %d \n",
                    i,Result);
            else
                printf("Image is a %d \n", Result);
            */

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

    if (!argc)
    {
        printf("\n The Precision is : %d",res);
        printf("%c \n\n", 37);
    }

    // Here we print the values of the weights

    //PrintValues(nInputs,nHiddenNodes,hWeights,"Hidden Weights");
    //PrintValues(nHiddenNodes, nOutputs,oWeights, "Output Weights");

    // Finally we save the values of Weights and output bias in a file.
    if (!argc)
    {
        OutputValues(filename, hWeights, oWeights, oBias, hBias, hLayer, oLayer);
    }

    for (int i = 0; i < nInputs;i++)
        free(hWeights[i]);
    free(hWeights);
    for (int i = 0; i < trainSets;i++)
        free(trainInput[i]);
    free(trainInput);
    free(oLayer);
    free(hLayer);
    free(oBias);
    free(hBias);
    for (int i = 0; i < nHiddenNodes;i++)
        free(oWeights[i]);
    free(oWeights);
    //free(memory);

	return Result;


}

/*
static int myCompare(const void* a, const void* b)
{
    // setting up rules for comparison
    return strcmp(*(const char**)a, *(const char**)b);
}

void sort(char* arr[], int n)
{
    // calling qsort function to sort the array
    // with the help of Comparator
    qsort(arr, n, sizeof(char*), myCompare);
}
*/

int Call()
{
    //char a[10][50] = {"final/SET4/00.png","final/SET4/53.png","final/SET4/42.png","final/SET4/46.png","final/SET4/49.png","final/SET4/06.png","final/SET4/45.png","final/SET4/12.png","final/SET4/68.png","final/SET4/78.png"};



    char** arr0 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr0[i] = (char*)malloc(STRING * sizeof(char));
    DIR *d;
    struct dirent *dir;
    d = opendir("training_data/0/");
    fill(arr0,d,dir,"training_data/0/");
    //int n = sizeof(arr0) / sizeof(arr0[0]);
    //sort(arr0, n);

    char** arr1 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr1[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/1/");
    fill(arr1,d,dir,"training_data/1/");
    //n = sizeof(arr1) / sizeof(arr1[0]);
    //sort(arr1, n);


    char** arr3 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr3[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/3/");
    fill(arr3,d,dir,"training_data/3/");
    //n = sizeof(arr3) / sizeof(arr3[0]);
    //sort(arr3, n);


    char** arr2 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr2[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/2/");
    fill(arr2,d,dir,"training_data/2/");
    //n = sizeof(arr2) / sizeof(arr2[0]);
    //sort(arr2, n);


    char** arr4 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr4[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/4/");
    fill(arr4,d,dir,"training_data/4/");
    //n = sizeof(arr4) / sizeof(arr4[0]);
    //sort(arr4, n);


    char** arr5 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr5[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/5/");
    fill(arr5,d,dir,"training_data/5/");
    //n = sizeof(arr5) / sizeof(arr5[0]);
    //sort(arr5, n);


    char** arr6 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr6[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/6/");
    fill(arr6,d,dir,"training_data/6/");
    //n = sizeof(arr6) / sizeof(arr6[0]);
    //sort(arr6, n);


    char** arr7 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr7[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/7/");
    fill(arr7,d,dir,"training_data/7/");
    //n = sizeof(arr7) / sizeof(arr7[0]);
    //sort(arr7, n);


    char** arr8 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr8[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/8/");
    fill(arr8,d,dir,"training_data/8/");
    //n = sizeof(arr8) / sizeof(arr8[0]);
    //sort(arr8, n);


    char** arr9 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr9[i] = (char*)malloc(STRING * sizeof(char));
    d = opendir("training_data/9/");
    fill(arr9,d,dir,"training_data/9/");
    //n = sizeof(arr9) / sizeof(arr9[0]);
    //sort(arr9, n);


    char** res = (char**)malloc(10 * sizeof(char*));
    for (int i = 0; i < 10; i++)
        res[i] = (char*)malloc(STRING * sizeof(char));
    for (int i = 0; i < IMAGES; i++)
    {
        strcpy(res[0],arr0[i]);
        strcpy(res[1],arr1[i]);
        strcpy(res[2],arr2[i]);
        strcpy(res[3],arr3[i]);
        strcpy(res[4],arr4[i]);
        strcpy(res[5],arr5[i]);
        strcpy(res[6],arr6[i]);
        strcpy(res[7],arr7[i]);
        strcpy(res[8],arr8[i]);
        strcpy(res[9],arr9[i]);
        ai(1,NULL,res);
        printf("iteration : %d\n",i);
    }
    /*
    printf("testing 1\n");
    res[0] = a[0];
    res[1] = a[1];
    res[2] = a[2];
    res[3] = a[3];
    res[4] = a[4];
    res[5] = a[5];
    res[6] = a[6];
    res[7] = a[7];
    res[8] = a[8];
    res[9] = a[9];
    ai(1,NULL,res);
    printf("testing 2\n");
    ai(2,"final/SET6/14.png",NULL);
    */

    for (int i = 0; i < IMAGES;i++)
    {
        free(res[i]);
        free(arr0[i]);
        free(arr1[i]);
        free(arr2[i]);
        free(arr3[i]);
        free(arr4[i]);
        free(arr5[i]);
        free(arr6[i]);
        free(arr7[i]);
        free(arr8[i]);
        free(arr9[i]);
    }
    free(res);
    free(arr0);
    free(arr1);
    free(arr2);
    free(arr3);
    free(arr4);
    free(arr5);
    free(arr6);
    free(arr7);
    free(arr8);
    free(arr9);
    return 0;
}
