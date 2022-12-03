#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

//init_sdl: initialize SDL and SDL_image before using
//exit if one of them fails
void init_sdl(){
	//Init SDL2
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Error initialiazing SDL: %s\n", SDL_GetError());
		exit(1);
	}
	//Init SDL_image
	if (&IMG_Init == 0){
		printf("Error initialiazing SDL_image");
		exit(1);
	}
}

//load_image: takes a path image and returns SDL_Surface* after loading
SDL_Surface* load_image(char* path){
	SDL_Surface* img = IMG_Load(path);

	if(!img){		//Checks if image exists
		printf("Couldn't load %s", path);
		exit(1);
	}
    SDL_Surface *res = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(img);

	return res;
}
