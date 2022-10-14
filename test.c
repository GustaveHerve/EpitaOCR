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

int main(int argc, char** argv){

    init_sdl();
	if (argc != 2)
		errx(2, "Wrong arguments\nUsage: ./binary filepath");
    SDL_Surface* test = load_image(argv[1]);
	

    unsigned int width = test->w;
	unsigned int height = test->h;

	//Convert surface to greyscale
	greyscale(test);

	//blur(test, 3);

    //thresholding
    otsu(test);

    //CANNY
	canny(test); 

	SDL_Surface* clo = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(test, NULL, clo, NULL);

	//dilate(test, 5);
	//erose(test, 5);
	closing(clo, 2);
	
	//double_thresholding(maxima, height, width, 0.50, 0.90);
	
    int rows = sqrt(height * height + width * width);
	 
	int angle_precision = 360;
    int *hough = calloc(angle_precision * rows, sizeof(int));

    //hough_init(hough, angle_precision, rows);
    hough_lines(test, angle_precision, 1, hough);

	Line *linesX = calloc(angle_precision * rows, sizeof(Line));
	Line *linesY = calloc(angle_precision * rows, sizeof(Line));

    TupleInt len_li = hough_filter_local(hough, rows, angle_precision, 300, 50, linesX, linesY);

    linesX = (Line *)realloc(linesX, len_li.x * sizeof(Line));
    linesY = (Line *)realloc(linesY, len_li.y * sizeof(Line));

	Segment segtest = get_segment(clo, &linesX[0]);

    SDL_FreeSurface(clo);
	TupleInt pt = {0,0};
      
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
		 	//SDL_RenderDrawPoint(renderer, x1, y1);
		 	//SDL_RenderDrawPoint(renderer, x1, y1);

        }
		
		for (int i = 0; i < len_li.x; i++){
			for (int j = 0; j < len_li.y; j++){
				TupleInt pt;
				if (!polar_intersection(&pt, linesX[i], linesY[j])){

					SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawLine(renderer, pt.x, pt.y-10, pt.x, pt.y + 10);
					SDL_RenderDrawLine(renderer, pt.x-10, pt.y, pt.x+10, pt.y);

				}
        	}
		}
    
        //copy texture to output device
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawPoint(renderer, pt.x, pt.y);
		 
        SDL_RenderPresent(renderer); 
        //it sends the renderer to window

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
	
    IMG_SavePNG(test, "/Users/gustave/Documents/c/images/briacpassicursed3.png");
    SDL_FreeSurface(test);

    //free(angles);
}
