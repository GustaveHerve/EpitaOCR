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
    SDL_Surface* image = load_image("/Users/gustave/Documents/c/images/image_06.jpeg");
    threshold(image, 0.70);
    IMG_SavePNG(image, "/Users/gustave/Documents/c/images/thresholdedsudokud.png");
}
