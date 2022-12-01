#ifndef MORPH_H
#define MORPH_H

void dilate_c(SDL_Surface *image, int kersize, int r[]);
void dilate(SDL_Surface *image, int kersize);
void erose_c(SDL_Surface *image, int kersize, int r[]);
void erose(SDL_Surface *image, int kersize);
void opening(SDL_Surface *image, int kersize);
void closing(SDL_Surface *image, int kersize);

#endif

