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
    IMG_SavePNG(test, "temp/guas.png");

	width = width;
	height = height; 

    IMG_SavePNG(test, "temp/blur.png");

	//Convert surface to greyscale
	if (verbose)
    	printf("Converting to greyscale..\n");

	if (verbose)
    	printf("Applying blur...\n");

	if (verbose)
    	printf("Thresholding...\n");

	//canny(test);
	adaptive_gaussthresholding(test, 11, 2);
    IMG_SavePNG(test, "temp/canny.png");

	dilate(test, 3);

    IMG_SavePNG(test, "temp/dilate.png");

	if (verbose)
    	printf("Dilating and closing...\n");

	SDL_Surface *blob = blob_detection(test);
    IMG_SavePNG(blob, "temp/blob.png");

	erose(blob, 5);
    IMG_SavePNG(blob, "temp/erose.png");

	//dilate(dilsur, 5);

    int rows = sqrt(height * height + width * width);

	int angle_precision = 360;

    int *hough = calloc(angle_precision * rows, sizeof(int));

	if (verbose)
    	printf("Detecting lines with Hough Transform...\n");

	hough_lines(blob, angle_precision, 1, hough);
	int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.5;
	Line *lines = calloc(angle_precision * rows, sizeof(Line));
	int line_nb = hough_filter(hough, rows, angle_precision, hough_threshold, lines);

	for (int i = 0; i < line_nb; i++)
		drawred(blob, &lines[i]);
	IMG_SavePNG(blob, "temp/lines.png");

	Square blobtest = get_blobs(lines, line_nb, (int)width, (int)height);
	draw_square(original, &blobtest);
	IMG_SavePNG(original, "temp/grid_detection.png");

	IMG_SavePNG(test, "temp/beforeextract.png");
	extract_cells(&blobtest, test, "temp/cells/");

    SDL_FreeSurface(test);
    SDL_FreeSurface(original);
    SDL_Quit();

}
