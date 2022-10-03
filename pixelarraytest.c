#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/pixel.h"
#include "include/image_processing.h"
#include "include/image_loading.h"
#include "include/matrix.h"
#include "include/utils.h"


int main(){

    init_sdl();
    SDL_Surface* test = load_image("/Users/gustave/Documents/c/images/birds.png");
    //SDL_Surface* test2 = load_image("/Users/gustave/Documents/c/images/image.png");
    unsigned int width = test->w;
	unsigned int height = test->h;

    //int *b = malloc(sizeof(int) * width * height);
	/*

	int *r1 = malloc(sizeof(int) * width * height);
    int *r2 = malloc(sizeof(int) * width * height);

    Uint8 *edges = malloc(sizeof(Uint8) * width * height);
    Uint8 *angles = malloc(sizeof(Uint8) * width * height);


	double kerx[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
    double kery[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};  
    double blur[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
    double gblur[] = {1/256, 4/256, 6/256, 4/256, 1/256, 4/256, 16/256,
    24/256, 16/256, 4/256, 6/256, 24/256, 36/256, 24/256, 6/256, 4/256,
    16/256, 24/256, 16/256, 4/256, 1/256, 4/256, 6/256, 4/256, 1/256};
    //double shar[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0};
    
    //BLURRING
    //convolution(test, blur, 5, 5, b, 1);
    //apply_convolution_int(test, b, (size_t)height, (size_t)width);
    //free(b);

    //thresholding
    otsu(test);
    
    //SOBELS X AND Y
	convolution(test, kerx, 3, 3, r1, 0);
    convolution(test, kery, 3, 3, r2, 0);

    //FUSE SOBELS X AND Y
    gradient(r1, r2, edges, angles, height, width);

    free(r1);
    free(r2);

    //CANNY NON MAXIMA
	Uint8 *maxima = calloc(width * height, sizeof(Uint8));
    non_maxima_suppr(edges, angles, height, width, maxima);
	free(edges);

	//double_thresholding(maxima, height, width, 0.50, 0.90);

    apply_convolution(test, maxima, (size_t)height, (size_t)width);

	*/ 
    

    int rows = sqrt(height * height + width * width);
	 
	int angle_precision = 90;
    int *hough = calloc(angle_precision * rows, sizeof(int));

    //hough_init(hough, angle_precision, rows);
    hough_lines(test, angle_precision, 100, hough);

    //Line *lines = malloc(sizeof(Line) * angle_precision * rows);
	//Line *linesX = calloc(angle_precision * rows, sizeof(Line));
	//Line *linesY = calloc(angle_precision * rows, sizeof(Line));

	//Line *lines = calloc(angle_precision * rows, sizeof(Line));
	/*    
   
    TupleInt len_li = hough_filter_local(hough, rows, angle_precision, 300, 50, linesX, linesY);

    linesX = (Line *)realloc(linesX, len_li.x * sizeof(Line));
    linesY = (Line *)realloc(linesY, len_li.y * sizeof(Line));

	TupleInt pt = {0,0};
	float ang1 = linesX[2].theta * M_PI / 180;
	float ang2 = linesY[3].theta * M_PI / 180;
	float ct1 = cos(ang1);
	float st1 = sin(ang1);
	float ct2 = cos(ang2);	
	float st2 = sin(ang2);
	float d = ct1 * st2 - st1*ct2;
	if (d!=0.0f){
		pt.x = (int)((st2*linesX[2].rho-st1*linesY[3].rho)/d);
		pt.y = (int)((-ct2*linesX[2].rho+ct1*linesY[3].rho)/d);
	}  */

   /*   
    SDL_Window *window = SDL_CreateWindow("Cookin'VR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,test->w,test->h,SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, test);

    int i = 1;
    SDL_Event event;

    
    while(i)
    {
        SDL_WaitEvent(&event);
        
        if(event.type == SDL_QUIT)    
            i = 0;
        
        SDL_Rect img_size = {0,0,test->w, test->h};

		//SDL_RenderSetScale(renderer, 0.667, 0.667);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
		  
        SDL_RenderCopy(renderer, texture, NULL, &img_size);
 	
        for (int i = 0; i < len_li.x; i++){
            int rho = linesX[i].rho;
            float theta = linesX[i].theta * M_PI / 180;
            double sinA = sin(theta);
            double cosA = cos(theta);
            float x0 = cosA * rho;
            float y0 = sinA * rho;
            int x1 = (x0 + rows * (-1 * sinA));
            int y1 = (y0 + rows * (cosA));
            int x2 = (x0 - rows * (-1 * sinA));
            int y2 = (y0 - rows * (cosA));


        	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			//if (lines[i].theta < 70)
           //variable 	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			SDL_RenderDrawPoint(renderer, x1, y1);
		 	//SDL_RenderDrawPoint(renderer, x1, y1);

        }	
		for (int i = 0; i < len_li.y; i++){
            int rho = linesY[i].rho;
            float theta = linesY[i].theta * M_PI / 180;
            double sinA = sin(theta);
            double cosA = cos(theta);
            float x0 = cosA * rho;
            float y0 = sinA * rho;
            int x1 = (x0 + rows * (-1 * sinA));
            int y1 = (y0 + rows * (cosA));
            int x2 = (x0 - rows * (-1 * sinA));
            int y2 = (y0 - rows * (cosA));


        	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			//if (lines[i].theta < 70)
           //variable 	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		 	SDL_RenderDrawPoint(renderer, x1, y1);
		 	//SDL_RenderDrawPoint(renderer, x1, y1);

        }
    
        //copy texture to output device
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawPoint(renderer, pt.x, pt.y);
		 
        SDL_RenderPresent(renderer); 
        //it sends the renderer to window

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
	*/
	   
    
    IMG_SavePNG(test, "/Users/gustave/Documents/c/images/briacpassicursed3.png");
    SDL_FreeSurface(test);

    //free(edges);
    //free(angles);
}
