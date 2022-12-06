#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "pixel.h"
#include "edge_detection.h"
#include "image_processing.h"
#include "image_loading.h"
#include "matrix.h"
#include "utils.h"
#include "geometry.h"
#include "grid_detection.h"
#include "morph.h"
#include "thresholding.h"
#include "grid_extraction.h"
#include "display.h"
#include "rotate_img.h"

int main(int argc, char** argv)
{
	if (argc <= 1)
		errx(1, "Must pass a file as argument");

    init_sdl();

    SDL_Surface* original = load_image(argv[1]);
    SDL_Surface* test = load_image(argv[1]);
	/*

	if (beforeresize->h > 800)
	{
		float coeff = (float)800 / beforeresize->h;
		int nw = beforeresize->w * coeff;
		SDL_Surface *tmp = SDL_CreateRGBSurface(0, nw, 800, 32, 0, 0, 0, 0);
		SDL_Surface *tmp_c = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
		SDL_FreeSurface(tmp);
		SDL_BlitScaled(beforeresize, NULL, tmp_c, NULL);
		original = SDL_CreateRGBSurface(0, nw, 800, 32, 0, 0, 0, 0);
		SDL_BlitSurface(tmp_c, NULL, original, NULL);
   		IMG_SavePNG(original, "temp/afterresize.png");
	}

	SDL_Surface *test_tmp = SDL_CreateRGBSurface(0, original->h, original->w, 32, 0, 0, 0, 0);
	SDL_Surface *test = SDL_ConvertSurfaceFormat(test_tmp, SDL_PIXELFORMAT_RGB888, 0);
	SDL_BlitSurface(original, NULL, test, NULL);
	SDL_FreeSurface(test_tmp);
	//SDL_Surface* test = load_image(argv[1]);
	*/

    unsigned int width = test->w;
	unsigned int height = test->h;

	greyscale(test);
	opening(test, 7);

	gauss_blur(test, 17, -1);

    IMG_SavePNG(test, ".temp/blur.png");

	//canny(test);
	adaptive_gaussthresholding(test, 13, 3);
    IMG_SavePNG(test, ".temp/canny.png");
	
	/*

	SDL_Surface *morph_tmp =
			SDL_CreateRGBSurface(0, test->w, test->h, 32, 0, 0, 0, 0);
    SDL_Surface *morph =
				SDL_ConvertSurfaceFormat(morph_tmp, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(morph_tmp);

	SDL_BlitSurface(test, NULL, morph, NULL);
	*/

	//dilate(test, 3);
	closing(test, 5);
	//erose(test, 3);
	//closing(test, 3);

    IMG_SavePNG(test, ".temp/dilate.png");

	SDL_Surface *blob = blob_detection(test);
    IMG_SavePNG(blob, ".temp/blob.png");

	erose(blob, 3);
    IMG_SavePNG(blob, ".temp/erose.png");

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
	IMG_SavePNG(blob, ".temp/lines.png");

	Square blobtest = get_blobs(lines, line_nb, (int)width, (int)height);
	free(lines);

	//draw_square(original, &blobtest);
	//IMG_SavePNG(original, "temp/grid_detection.png");

	//erose(test, 3);
	SDL_Surface *homo = homographic_Transform(original, blobtest);
	IMG_SavePNG(homo, ".temp/homo.png");
	//greyscale(homo);
	//gauss_blur(homo, 25, -1);
	//adaptive_gaussthresholding(homo, 7, 2);

	IMG_SavePNG(homo, ".temp/otsu.png");
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
	extract_cells(&homorect, homo, ".temp/cells/");

    SDL_FreeSurface(test);
    SDL_FreeSurface(original);
    SDL_FreeSurface(homo);
    SDL_Quit();

}
