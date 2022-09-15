#ifndef PIXEL_H
#define PIXEL_H

#include <SDL2/SDL.h>

Uint8* get_pixel_ref(SDL_Surface* image, unsigned x, unsigned y);

Uint32 get_pixel(SDL_Surface *image, unsigned x, unsigned y);

void replace_pixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel);

#endif