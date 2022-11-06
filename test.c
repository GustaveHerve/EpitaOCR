#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "include/pixel.h"
#include "include/edge_detection.h"
#include "include/image_processing.h"
#include "include/image_loading.h"
#include "include/matrix.h"
#include "include/utils.h"
#include "include/geometry.h"
#include "include/grid_detection.h"
#include "include/morph.h"
#include "include/thresholding.h"
#include "include/grid_extraction.h"

int main(int argc, char** argv){

    init_sdl();
	if (argc != 2)
		errx(2, "Wrong arguments\nUsage: ./binary filepath");
    SDL_Surface* test = load_image(argv[1]);
	//SDL_LockSurface(test);

    unsigned int width = test->w;
	unsigned int height = test->h;

	//Convert surface to greyscale
	greyscale(test);

	//blur(test, 3);

	//adaptive_thresholding(test, 11, 11);

    //otsu(test);
	//invert(test);
	canny(test);
	//erose(test, 3);
	//dilate(test, 3);

	SDL_Surface* dilsur = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(test, NULL, dilsur, NULL);
	dilate(dilsur, 5);

    int rows = sqrt(height * height + width * width);

	int angle_precision = 180;

    int *hough = calloc(angle_precision * rows, sizeof(int));
	//hough_lines_gradient(test, angle_precision, can.edges, can.angles, hough);

	hough_lines(dilsur, angle_precision, 1, hough);

	Line *linesX = calloc(angle_precision * rows, sizeof(Line));
	Line *linesY = calloc(angle_precision * rows, sizeof(Line));

	int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.5;

    TupleInt len_li = hough_filter_local(hough, rows, angle_precision, hough_threshold, 1, 0, linesX, linesY);

	free(hough);

    linesX = (Line *)realloc(linesX, len_li.x * sizeof(Line));
    linesY = (Line *)realloc(linesY, len_li.y * sizeof(Line));

	Line *cacay = malloc(len_li.y * sizeof(Line));
	Line *cacax = malloc(len_li.x * sizeof(Line));

	len_li.y =  merge_lines(linesY, len_li.y, 30, cacay);
	len_li.x =  merge_lines(linesX, len_li.x, 30, cacax);

	linesX = cacax;
	linesY = cacay;
	

	Line* gridX = calloc(len_li.x, sizeof(Line));
	Line* gridY = calloc(len_li.y, sizeof(Line));

	int xtemp = get_grid(linesX, len_li.x, 16, gridX);
	int ytemp = get_grid(linesY, len_li.y, 16, gridY);

	Square *squares = calloc(81, sizeof(Square));
	int len = get_squares(gridX, gridY, squares);
	save_squares(squares, len, test);


    SDL_FreeSurface(dilsur);
      
    SDL_Window *window = SDL_CreateWindow("Cookin'VR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,test->w,test->h,SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(2, "couldn't create window");
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

		//SDL_RenderSetScale(renderer, 0.5, 0.5);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
		  
        SDL_RenderCopy(renderer, texture, NULL, &img_size);

 	
        for (int i = 0; i < xtemp; i++){
            int rho = gridX[i].rho;
            float theta = gridX[i].theta * M_PI / 180;
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
		for (int i = 0; i < ytemp; i++){
            int rho = gridY[i].rho;
            float theta = gridY[i].theta * M_PI / 180;
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

        }
		
	/*	
		for (int i = 0; i < xtemp; i++){
			for (int j = 0; j < ytemp; j++){
				TupleInt pt;
				if (!polar_intersection(&pt, gridX[i], gridY[j])){

					SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawLine(renderer, pt.x, pt.y-10, pt.x, pt.y + 10);
					SDL_RenderDrawLine(renderer, pt.x-10, pt.y, pt.x+10, pt.y);

				}
        	}
		}
		*/
	
   
        //copy texture to output device
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawPoint(renderer, pt.x, pt.y);
		 
        SDL_RenderPresent(renderer); 
        //it sends the renderer to window

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
   	IMG_SavePNG(test, "/Users/gustave/Documents/c/images/final.png");
	
    SDL_FreeSurface(test);
	//SDL_FreeSurface(clo);
	free(linesX);
	free(linesY);
	//free(gridX);
	//free(gridY);

}

void test3(char *path){

    SDL_Surface* test = load_image(path);
	SDL_LockSurface(test);

    unsigned int width = test->w;
	unsigned int height = test->h;

	greyscale(test);

	canny(test);

    int rows = sqrt(height * height + width * width);

	int angle_precision = 180;

    int *hough = calloc(angle_precision * rows, sizeof(int));

	hough_lines(test, angle_precision, 1, hough);

	Line *linesX = calloc(angle_precision * rows, sizeof(Line));
	Line *linesY = calloc(angle_precision * rows, sizeof(Line));

	int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.5;

    TupleInt len_li = hough_filter_local(hough, rows, angle_precision, hough_threshold, 1, 3, linesX, linesY);

	free(hough);

    linesX = (Line *)realloc(linesX, len_li.x * sizeof(Line));
    linesY = (Line *)realloc(linesY, len_li.y * sizeof(Line));

	Line *tempy = malloc(len_li.y * sizeof(Line));
	Line *tempx = malloc(len_li.x * sizeof(Line));

	len_li.y =  merge_lines(linesY, len_li.y, 30, tempy);
	len_li.x =  merge_lines(linesX, len_li.x, 30, tempx);

	linesX = tempx;
	linesY = tempy;
	

	Line* gridX = calloc(len_li.x, sizeof(Line));
	Line* gridY = calloc(len_li.y, sizeof(Line));

	int xtemp = get_grid(linesX, len_li.x, 16, gridX);
	int ytemp = get_grid(linesY, len_li.y, 16, gridY);

	Square *squares = calloc(81, sizeof(Square));
	int len = get_squares(gridX, gridY, squares);
	save_squares(squares, len, test);

	SDL_UnlockSurface(test);
	free(linesX);
	free(linesY);

	free(gridX);
	free(gridY);
}
