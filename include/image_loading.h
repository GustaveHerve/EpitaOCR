#ifndef IMAGE_LOADING_H
#define IMAGE_LOADING_H

#include <SDL2/SDL.h>

void init_sdl();

SDL_Surface* load_image(char* path);

#endif
