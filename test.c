#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
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
#include "include/display.h"
#include "include/rotate_img.h"

int main(int argc, char** argv)
{
    init_sdl();
	if (argc < 2 || argc > 3)
		errx(2, "Wrong arguments\nUsage: ./binary filepath -option");

	int verbose = 0;

	if (argc == 3)
	{
		if (strcmp(argv[2], "-v") == 0)
			verbose = 1;
		else
			errx(2, "Wrong arguments\nUsage: ./binary filepath -option");
	}

	SDL_Surface* test = load_image(argv[1]);
    SDL_Surface* original = load_image(argv[1]);

    unsigned int width = test->w;
	unsigned int height = test->h;

    
	greyscale(test);

	//dilate(test, 3);
	//erose(test, 5);
	gauss_blur(test, 9, 2.5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
	//blur(test, 5);
    IMG_SavePNG(test, "temp/guas.png");

	width = width - 18;
	height = height - 18; 
	SDL_Surface* blurfix = SDL_CreateRGBSurface(0, width, 
												height, 32, 0, 0, 0, 0);
 	blurfix = SDL_ConvertSurfaceFormat(blurfix, SDL_PIXELFORMAT_RGB888, 0);
	SDL_Rect blurfixrect = { 9, 9, width, height };
	SDL_BlitSurface(test, &blurfixrect, blurfix, NULL);
	test = blurfix;

    IMG_SavePNG(test, "temp/blur.png");

    IMG_SavePNG(test, "temp/dilate.png");
	//Convert surface to greyscale
	if (verbose)
    	printf("Converting to greyscale..\n");

	if (verbose)
    	printf("Applying blur...\n");

	if (verbose)
    	printf("Thresholding...\n");

	//canny(test);
	adaptive_gaussthresholding(test, 11, 3);
    IMG_SavePNG(test, "temp/canny.png");


	if (verbose)
    	printf("Dilating and closing...\n");

	//dilate(dilsur, 5);

    int rows = sqrt(height * height + width * width);

	int angle_precision = 360;

    int *hough = calloc(angle_precision * rows, sizeof(int));

	if (verbose)
    	printf("Detecting lines with Hough Transform...\n");

	hough_lines(test, angle_precision, 1, hough);
	int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.5;
	Line *lines = calloc(angle_precision * rows, sizeof(Line));
	int line_nb = hough_filter(hough, rows, angle_precision, hough_threshold, lines);

	for (int i = 0; i < line_nb; i++)
		drawred(test, &lines[i]);
	IMG_SavePNG(test, "temp/lines.png");

	Square blobtest = get_blobs(lines, line_nb, (int)width, (int)height);

	/*


	Line *linesX = calloc(angle_precision * rows, sizeof(Line));
	Line *linesY = calloc(angle_precision * rows, sizeof(Line));


    TupleInt len_li = hough_filter_local(hough, rows, angle_precision,
			hough_threshold, 1, 0, linesX, linesY);

    linesX = (Line *)realloc(linesX, len_li.x * sizeof(Line));
    linesY = (Line *)realloc(linesY, len_li.y * sizeof(Line));

	Line *mergey = malloc(len_li.y * sizeof(Line));
	Line *mergex = malloc(len_li.x * sizeof(Line));

	len_li.y =  merge_lines(linesY, len_li.y, 40, mergey);
	len_li.x =  merge_lines(linesX, len_li.x, 40, mergex);

	free(linesX);
	free(linesY);
	//linesX = mergex;
	//linesY = mergey;
	if (verbose)
    	printf("Detecting grid...\n");

	Segment *xseg = get_segments(dilsur, mergex, len_li.x);
	Segment *yseg = get_segments(dilsur, mergey, len_li.y);

	if (verbose)
    	printf("Extracting grid squares...\n");
	Segment *grid = get_grid_seg(xseg, yseg, len_li);

	Square *sq = get_squares_seg(grid);

	save_squares_seg(sq, test, "temp/");

    SDL_FreeSurface(dilsur);
    free(sq);
    free(grid);

    SDL_Window *window = SDL_CreateWindow("Cookin'VR",SDL_WINDOWPOS_UNDEFINED
			,SDL_WINDOWPOS_UNDEFINED,test->w,test->h,SDL_WINDOW_RESIZABLE);
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

        for (int i = 0; i < len_li.x; i++)
		{
            int rho = mergex[i].rho;
            float theta = mergex[i].theta * M_PI / 180;
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
			SDL_RenderDrawPoint(renderer, x1, y1);

        }
		for (int i = 0; i < len_li.y; i++)
		{
            int rho = mergey[i].rho;
            float theta = mergey[i].theta * M_PI / 180;
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
		*/
	/*
		for (int i = 0; i < xtemp; i++){
			for (int j = 0; j < ytemp; j++){
				TupleInt pt;
				if (!polar_intersection(&pt, gridX[i], gridY[j])){

					SDL_SetRenderDrawColor(renderer, 0, 255
					, 255, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawLine(renderer, pt.x
					, pt.y-10, pt.x, pt.y + 10);
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

	*/


    //SDL_DestroyTexture(texture);
    //SDL_DestroyWindow(window);
    //SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(test);
    SDL_FreeSurface(original);
    SDL_Quit();

    //free(yseg);
    //free(xseg);

}

