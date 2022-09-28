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
    SDL_Surface* test = load_image("/Users/gustave/Documents/c/images/cacatest.png");
    //SDL_Surface* test2 = load_image("/Users/gustave/Documents/c/images/image.png");
    unsigned int width = test->w;
	unsigned int height = test->h;

    int *b = malloc(sizeof(int) * width * height);

	int *r1 = malloc(sizeof(int) * width * height);
    int *r2 = malloc(sizeof(int) * width * height);

    Uint8 *edges = malloc(sizeof(Uint8) * width * height);
    Uint8 *angles = malloc(sizeof(Uint8) * width * height);


	double kerx[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
    double kery[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};  
    double blur[] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
    //double gblur[] = {1/256, 4/256, 6/256, 4/256, 1/256, 4/256, 16/256,
    //24/256, 16/256, 4/256, 6/256, 24/256, 36/256, 24/256, 6/256, 4/256,
    //16/256, 24/256, 16/256, 4/256, 1/256, 4/256, 6/256, 4/256, 1/256};
    //double shar[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0};
    
    //BLURRING
    //convolution(test, blur, 3, 3, b, 0);
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
	Uint8 *maxima = malloc(sizeof(Uint8) * width * height);
    non_maxima_suppr(edges, angles, height, width, maxima);

    apply_convolution(test, maxima, (size_t)height, (size_t)width);
    

    int rows = sqrt(height * height + width * width);
	int angle_precision = 181;
    int *hough = malloc(sizeof(int) * angle_precision * rows);

    hough_init(hough, angle_precision, rows);
    hough_lines(test, angle_precision, 1,  hough);

    //Line *lines = malloc(sizeof(Line) * angle_precision * rows);
	Line *lines = calloc(angle_precision * rows, sizeof(Line));

    int len_li = hough_filter(hough, rows, angle_precision, 400, lines);
    //lines = (Line *)realloc(lines, len_li * sizeof(Line));
	//int newlen_li = hough_average(lines, len_li, 20);
    
    SDL_Window *window = SDL_CreateWindow("Cookin'VR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,test->w/1.5,test->h/1.5,SDL_WINDOW_RESIZABLE);
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

		SDL_RenderSetScale(renderer, 0.667, 0.667);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &img_size);
        for (int i = 0; i < len_li; i++){
            int rho = lines[i].rho;
            float theta = lines[i].theta * M_PI / 180;
            double sinA = sin(theta);
            double cosA = cos(theta);
            float x0 = cosA * rho;
            float y0 = sinA * rho;
            int x1 = (x0 + rows * (-1 * sinA));
            int y1 = (y0 + rows * (cosA));
            int x2 = (x0 - rows * (-1 * sinA));
            int y2 = (y0 - rows * (cosA));


        	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		 	//SDL_RenderDrawPoint(renderer, x0, y0);
		 	//SDL_RenderDrawPoint(renderer, x1, y1);

        }
    
        //copy texture to output device
        
        SDL_RenderPresent(renderer); 
        //it sends the renderer to window

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    IMG_SavePNG(test, "/Users/gustave/Documents/c/images/briacpassicursed3.png");
    SDL_FreeSurface(test);

    free(edges);
    free(angles);
}
