#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "include/pixel.h"
#include "include/image_processing.h"
#include "include/image_loading.h"

int main(int argc, char** argv){
	if (argc != 3){
		printf("Wrong number of arguments\n");
		printf("Usage: ./test_greyscale imagePath.(any format) 
				newgreyimagePath.png\n");
		exit(2);
	}

	init_sdl();
	char* path = argv[1];
	char* savepath = argv[2];

	SDL_Surface* image = load_image(path);
	greyscale(image);

	IMG_SavePNG(image, savepath);
}
