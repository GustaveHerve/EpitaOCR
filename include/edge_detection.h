#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <SDL2/SDL.h>

void sobel_c(SDL_Surface* surf, Uint8* edges, Uint8* angles);
void sobel(SDL_Surface* surf);


#endif
