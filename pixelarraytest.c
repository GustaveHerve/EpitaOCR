#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/pixel.h"
#include "include/image_processing.h"
#include "include/image_loading.h"
#include "include/matrix.h"


int main(){

    init_sdl();
    SDL_Surface* test = load_image("/Users/gustave/Documents/c/images/town.jpg");
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
    
    //convolution(test, blur, 3, 3, b, 1);
    //apply_convolution(test, b, (size_t)height, (size_t)width);
    //free(b);

    //threshold(test, 0.5);
    otsu(test);
    
	//convolution(test, kerx, 3, 3, r1, 0);
    //convolution(test, kery, 3, 3, r2, 0);

    ///gradient(r1, r2, edges, angles, height, width);

    //free(r1);
    //free(r2);

    //non_maxima_suppr(edges, angles, height, width);

    //apply_convolution(test, edges, (size_t)height, (size_t)width);
    //apply_convolution(test, r, (size_t)height, (size_t)width);
    //apply_convolution(test2, r2, (size_t)height, (size_t)width);
    //IMG_SavePNG(test, "/Users/gustave/Documents/c/images/ouiX.jpeg");
    //IMG_SavePNG(test2, "/Users/gustave/Documents/c/images/ouiY.jpeg");
    
    IMG_SavePNG(test, "/Users/gustave/Documents/c/images/briaccursed.png");
    //IMG_SavePNG(test2, "/Users/gustave/Documents/c/images/valveSY.png");


    free(edges);
    free(angles);
}
