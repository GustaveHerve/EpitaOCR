#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "include/pixel.h"
#include "include/greyscale.h"
#include "include/image_loading.h"
#include "include/matrix.h"

void apply_convolution(SDL_Surface *image, Uint8 r[], size_t rows, size_t cols){
   
    for (size_t i = 0; i < rows; i++){
        for (size_t j= 0; j < cols; j++){
            Uint32 pixel = SDL_MapRGB(image->format, r[i*cols + j], r[i*cols + j],
					r[i*cols + j]);
            replace_pixel(image, j, i, pixel);
        }
    }
}

int main(){

    init_sdl();
    SDL_Surface* test = IMG_Load("/Users/gustave/Documents/c/images/cacatest.png");
    //SDL_Surface* test2 = IMG_Load("/Users/gustave/Documents/c/images/isseygr.jpg");
    unsigned int width = test->w;
	unsigned int height = test->h;

	Uint8 *r = malloc(sizeof(Uint8) * width * height);
    Uint8 *r2 = malloc(sizeof(Uint8) * width * height);
    Uint8 *res = malloc(sizeof(Uint8) * width * height);

	double kerx[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
    double kery[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};  
    //double shar[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0};

	convolution(test, kerx, 3, 3, r, 0);
    convolution(test, kery, 3, 3, r2, 0);

    gradient(r, r2, res, height, width);

    apply_convolution(test, res, (size_t)height, (size_t)width);
    //apply_convolution(test, r, (size_t)height, (size_t)width);
    //apply_convolution(test2, r2, (size_t)height, (size_t)width);
    //IMG_SavePNG(test, "/Users/gustave/Documents/c/images/ouiX.jpeg");
    //IMG_SavePNG(test2, "/Users/gustave/Documents/c/images/ouiY.jpeg");
    IMG_SavePNG(test, "/Users/gustave/Documents/c/images/testfusion.jpeg");

}
