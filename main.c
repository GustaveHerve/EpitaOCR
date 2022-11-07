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
    SDL_Surface *img = load_image(argv[1]);
    double angle = atof(argv[2]);
    if(angle == 360)
    {
        open_img_window(argv[1]);
	return 0;
    }
    else
    {
       // if (angle > 180)
//		angle = -(360-angle);
//        if (angle == 90 || angle == 180)
            rotate_img90(img, angle);
//        else
  //          rotate_img(img, angle); //atof converts a "string" to a double
    }
    open_img_window("image_rotated.png");
    SDL_FreeSurface(img);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
