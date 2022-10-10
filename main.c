#include <stdio.h>
#include <err.h>
#include <errno.h>
#include "include/rotate_img.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/sdaile.h"
#include "include/image_loading.h"

int main (int argc, char* argv[])
{

    init_sdl();
    SDL_Surface *img = load_image("/Users/gustave/Documents/c/images/image_03.jpeg");
    rotate_img(img, M_PI); //atof converts a "string" to a double
    open_img_window("image_rotated.png");

}
