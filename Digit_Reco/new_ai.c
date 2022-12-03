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
#define nHiddenNodes 175
#define filename "Brain"
#define IMAGES 100
#define STRING 25

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

     for (int j = 0; j < nHiddenNodes; j++)
         hBias[j] = initWeights();

     for (int j = 0; j < nOutputs; j++)
         oLayer[j] = initWeights();

     for (int j = 0; j < nHiddenNodes; j++)                                         
         hLayer[j] = initWeights(); 

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
 

     for (int j = 0; j < nHiddenNodes; j++)
          fprintf(fp,"%lf\n",hLayer[j]);
 

     for (int j = 0; j < nOutputs; j++)
          fprintf(fp,"%lf\n",oLayer[j]);
     

     fclose(fp);


}

void fill(char **arr, DIR *d, struct dirent *dir, char *name)
{
    int i = 0;
    while ((dir = readdir(d)) != NULL && i < IMAGES)
    {
        if (strstr(dir->d_name, "png") != NULL)
        {
            char fn[20];
            strcpy( fn, name);
            char* s = strcat(fn,dir->d_name);
            strcpy(arr[i],s);
            i++;
        }
    }
    closedir(d);
}

int ai(int argc, char *argv, char **im)
{
    /*
    printf("%s\n",im[0]);
    printf("%s\n",im[1]);
    printf("%s\n",im[2]);
    printf("%s\n",im[3]);
    printf("%s\n",im[4]);
    printf("%s\n",im[5]);
    printf("%s\n",im[6]);
    printf("%s\n",im[7]);
    printf("%s\n",im[8]);
    printf("%s\n",im[9]);
    */    
    if (argc > 2)
    {
        fprintf(stderr, "To many arguments \n");
        return 1;
    }
    //printf("New it 1 \n"); 
    argc--;
	double lr = 0.1f; //Learning Rate

	//double (*hLayer) = malloc(sizeof(double[nHiddenNodes]));;
    double* hLayer = (double*)malloc(nHiddenNodes * sizeof(double));

    //if (hLayer == NULL)
        //printf("Couscous 1 !\n"); 
    
    //printf("New it 2 \n"); 

    //double hLayer[nHiddenNodes];
    //double oLayer[nOutputs];  
	//double (*oLayer) = malloc(sizeof(double[nOutputs]));
    double* oLayer = (double*)malloc(nOutputs * sizeof(double));

    //if (oLayer == NULL)                                                         
        //printf("Couscous 2 !\n"); 

	//double (*hBias) = malloc(sizeof(double[nHiddenNodes]));
    double* hBias = (double*)malloc(nHiddenNodes * sizeof(double));

    //if (hBias == NULL)
        //printf("Couscous 3 !\n"); 
    //double hBias[nHiddenNodes];
    //
	//double (*oBias) = malloc(sizeof(double[nOutputs]));
    double* oBias = (double*)malloc(nOutputs * sizeof(double)); 

    //if (oBias == NULL)
       //printf("Couscous 4 !\n"); 

    //printf("New it 3 \n"); 
    //double oBias[nOutputs];  

    int numberOfTimes = 100;
    if (argc)
        numberOfTimes = 1;

    int memo_val = 0;
    int Result;
    
    int trainSets = 10;
    if (argc)
        trainSets = 1;

    double memory[numberOfTimes*trainSets];
    
    //printf("New it 4 \n"); 

	//double (*hWeights)[nHiddenNodes] =
        //malloc(sizeof(double[nInputs][nHiddenNodes]));
    double** hWeights = (double**)malloc(nInputs * sizeof(double*));
    for (int i = 0; i < nInputs; i++)
        hWeights[i] = (double*)malloc(nHiddenNodes * sizeof(double));
    //if (hWeights == NULL)
        //printf("Couscous 6 !\n"); 

    //printf("New it 5 \n"); 

	//double (*oWeights)[nOutputs] =
        //malloc(sizeof(double[nHiddenNodes][nOutputs]));
    
    double** oWeights = (double**)malloc(nHiddenNodes * sizeof(double*));
    for (int i = 0; i < nHiddenNodes; i++)
        oWeights[i] = (double*)malloc(nOutputs * sizeof(double));

    //if (oWeights == NULL)                                                         
        //printf("Couscous 7 !\n");    

    //printf("New it 6 \n"); 

	//double (*trainInput)[nInputs] =
        //malloc(sizeof(double[trainSets][nInputs]));
    
    double** trainInput = (double**)malloc(trainSets * sizeof(double*));
    for (int i = 0; i < trainSets; i++)
        trainInput[i] = (double*)malloc(nInputs * sizeof(double));

    //if (trainInput == NULL)                                                         
        //printf("Couscous 8 !\n");     
    

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
        /*
        for (int i = 0; i < nInputs;i++)
          printf("%lf", trainInput[0][i]);
          */
     }

     else 
     {   
     image = load_image(im[0]);
     parcours_pixel(image,trainInput[0]);
     image = load_image(im[1]);
     parcours_pixel(image,trainInput[1]);
     image = load_image(im[2]);
     parcours_pixel(image,trainInput[2]);
     image = load_image(im[3]);
     parcours_pixel(image,trainInput[3]);
     image = load_image(im[4]);
     parcours_pixel(image,trainInput[4]);
     image = load_image(im[5]);
     parcours_pixel(image,trainInput[5]);
     image = load_image(im[6]);
     parcours_pixel(image,trainInput[6]);
     image = load_image(im[7]);
     parcours_pixel(image,trainInput[7]);
     /*
     for (int i = 0; i < nInputs;i++)
         printf("%lf", trainInput[7][i]);
     */
     image = load_image(im[8]);
     parcours_pixel(image,trainInput[8]);
     image = load_image(im[9]);
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

	return Result;


}

int Call()
{
    //char (*arr0)[40] = malloc(sizeof(char[10][40]));
    //char arr0[IMAGES][40];
    char** arr0 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr0[i] = (char*)malloc(STRING * sizeof(char));
    DIR *d;
    struct dirent *dir;
    d = opendir("training_data/0/");
    fill(arr0,d,dir,"training_data/0/");

    //char (*arr1)[40] = malloc(sizeof(char[10][40]));
    //char arr1[IMAGES][40];
    printf("Nani 1 \n");
    char** arr1 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr1[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d1;
    //struct dirent *dir1;
    printf("Nani 2 \n"); 
    d = opendir("training_data/1/");
    fill(arr1,d,dir,"training_data/1/");

    //char (*arr3)[40] = malloc(sizeof(char[10][40]));
    //char arr3[IMAGES][40];
    char** arr3 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr3[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d3;
    //struct dirent *dir3;
    d = opendir("training_data/3/");
    fill(arr3,d,dir,"training_data/3/");

    //char (*arr2)[40] = malloc(sizeof(char[10][40]));
    //char arr2[IMAGES][40];
    char** arr2 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr2[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d2;
    //struct dirent *dir2;
    d = opendir("training_data/2/");
    fill(arr2,d,dir,"training_data/2/");

    //char (*arr4)[40] = malloc(sizeof(char[10][40]));
    //char arr4[IMAGES][40];
    char** arr4 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr4[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d4;
    //struct dirent *dir4;
    d = opendir("training_data/4/");
    fill(arr4,d,dir,"training_data/4/");

    //char (*arr5)[40] = malloc(sizeof(char[10][40]));
    //char arr5[IMAGES][40];
    char** arr5 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr5[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d5;
    //struct dirent *dir5;
    d = opendir("training_data/5/");
    fill(arr5,d,dir,"training_data/5/");

    //char (*arr6)[40] = malloc(sizeof(char[10][40]));
    //char arr6[IMAGES][40];
    char** arr6 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr6[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d6;
    //struct dirent *dir6;
    d = opendir("training_data/6/");
    fill(arr6,d,dir,"training_data/6/");

    //char (*arr7)[40] = malloc(sizeof(char[10][40]));
    //char arr7[IMAGES][40];
    char** arr7 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr7[i] = (char*)malloc(STRING * sizeof(char));                            
    //DIR *d7;                                                                    
    //struct dirent *dir7;                                                        
    d = opendir("training_data/7/");                                    
    fill(arr7,d,dir,"training_data/7/");                               
                                                                                 
    //char (*arr8)[40] = malloc(sizeof(char[10][40]));
    //char arr8[IMAGES][40];
    char** arr8 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr8[i] = (char*)malloc(STRING * sizeof(char));                            
    //DIR *d8;                                                                    
    //struct dirent *dir8;                                                        
    d = opendir("training_data/8/");                                    
    fill(arr8,d,dir,"training_data/8/");                               
                                                                                
    //char (*arr9)[40] = malloc(sizeof(char[10][40]));
    //char arr9[IMAGES][40];
    char** arr9 = (char**)malloc(IMAGES * sizeof(char*));
    for (int i = 0; i < IMAGES; i++)
        arr9[i] = (char*)malloc(STRING * sizeof(char));
    //DIR *d9;                                                                    
    //struct dirent *dir9;                                                        
    d = opendir("training_data/9/");                                    
    fill(arr9,d,dir,"training_data/9/");                               
                                                                                
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
    }
    return 1;    
    //free(arr0);                                                                 
    //free(arr1);                                                                 
    //free(arr2);                                                                 
    //free(arr3);                                                                 
    //free(arr4);                                                                 
    //free(arr5);                                                                 
    //free(arr6);                                                                 
    //free(arr7);                                                                 
    //free(arr8);                                                                 
    //free(arr9);
    //free(res);    
}                          



