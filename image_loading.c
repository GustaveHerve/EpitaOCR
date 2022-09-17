#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

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
	SDL_Surface* image = IMG_Load(path);

	if(!image){		//Checks if image exists
		printf("Couldn't load %s", path);
		exit(1);
	}

	return image;
}
