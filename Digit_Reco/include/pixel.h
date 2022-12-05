#ifndef PIXEL_H
#define PIXEL_H

#include <SDL2/SDL.h>

Uint8* get_pixel_ref(SDL_Surface* image, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *image, unsigned x, unsigned y);
void replace_pixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel);
void get_pixel_array(SDL_Surface *image, Uint32 r[]);
void get_binarray(SDL_Surface *image, Uint8 *r);
void binarraycpy(Uint8 *a, Uint8 *r, int size);
void binarr_to_surf(Uint8 *arr, SDL_Surface *dest, int size);

#endif
