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

	SDL_Surface* test = load_image(argv[1]);
    SDL_Surface* original = load_image(argv[1]);

	/*
	if (test->h > 800)
	{
		float coeff = 800 / test->h;
		int nw = test->w * coeff;
		SDL_Surface *tmp = SDL_CreateRGBSurface(0, nw, 800, 32, 0, 0, 0, 0);
		SDL_Surface *tmp_c = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
		SDL_FreeSurface(tmp);
		SDL_BlitScaled(test, NULL, tmp_c, NULL);
		test = SDL_CreateRGBSurface(0, nw, 800, 32, 0, 0, 0, 0);
		SDL_BlitSurface(tmp_c, NULL, test, NULL);
	}
	*/

    unsigned int width = test->w;
	unsigned int height = test->h;

	greyscale(test);

	gauss_blur(test, 11, 2.5);

	width = width;
	height = height; 

	//canny(test);
	adaptive_gaussthresholding(test, 11, 2);
    IMG_SavePNG(test, "temp/canny.png");

	dilate(test, 3);
	//erose(test, 3);
	//closing(test, 3);

    IMG_SavePNG(test, "temp/dilate.png");

	SDL_Surface *blob = blob_detection(test);
    IMG_SavePNG(blob, "temp/blob.png");

	erose(blob, 3);
    IMG_SavePNG(blob, "temp/erose.png");

	//dilate(dilsur, 5);

    int rows = sqrt(height * height + width * width);
	int angle_precision = 360;
    int *hough = calloc(angle_precision * rows, sizeof(int));
	hough_lines(blob, angle_precision, 1, hough);

	int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.7;
	Line *lines = calloc(angle_precision * rows, sizeof(Line));
	int line_nb = hough_filter(hough, rows, angle_precision, hough_threshold, lines);
	free(hough);

	for (int i = 0; i < line_nb; i++)
		drawred(blob, &lines[i]);
	IMG_SavePNG(blob, "temp/lines.png");

	Square blobtest = get_blobs(lines, line_nb, (int)width, (int)height);
	free(lines);

	//draw_square(original, &blobtest);
	//IMG_SavePNG(original, "temp/grid_detection.png");

	//erose(test, 3);
	erose(test, 3);
	SDL_Surface *homo = homographic_Transform(test, blobtest);
	IMG_SavePNG(homo, "temp/homo.png");

	//gauss_blur(homo, 11, 2.5);
	//adaptive_gaussthresholding(homo, 11, 3);
	IMG_SavePNG(homo, "temp/otsu.png");
	//closing(homo, 3);
	Square homorect;
	TupleInt a = { 0, 0 };
	TupleInt b = { homo->w - 1, 0 };
	TupleInt c = { homo->w - 1, homo->h - 1 };
	TupleInt d = { 0, homo->h - 1 };
	homorect.NW = a;
	homorect.NE = b;
	homorect.SE = c;
	homorect.SW = d;
	extract_cells(&homorect, homo, "temp/cells/");

    SDL_FreeSurface(test);
    SDL_FreeSurface(original);
    SDL_FreeSurface(homo);
    SDL_Quit();

}
