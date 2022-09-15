#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void init_sdl(){
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Error initialiazing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (IMG_Init == 0){
		printf("Error initialiazing SDL_image");
		exit(1);
	}

}

SDL_Surface* load_image(char* path){

	SDL_Surface* image = IMG_Load(path);

	if(!image){
		printf("Couldn't load %s", path);
		exit(1);
	}

	return image;

}

Uint8* get_pixel_loc(SDL_Surface* image, unsigned x, unsigned y){
	return (Uint8*)image->pixels + y * image->pitch + 
		x * image->format->BytesPerPixel;
}

Uint32 get_pixel(SDL_Surface *image, unsigned x, unsigned y){

	Uint8 *p = get_pixel_loc(image, x, y);

	switch (image->format->BytesPerPixel){
		case 1:
			return *p;
		case 2:
			return *(Uint16*)p;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
	}
	return 0;
}

void replace_pixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel){
	Uint8 *p = get_pixel_loc(image, x, y);

	switch(image->format->BytesPerPixel){
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void greyscale(SDL_Surface *image){
	unsigned int w = image->w;
	unsigned int h = image->h;

	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r, g, b = (0, 0, 0);
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			Uint8 average = 0.3*r + 0.59*g + 0.11*b;
			Uint32 newpixel = SDL_MapRGB(image->format, average, average,
					average);
			replace_pixel(image, j, i, newpixel);

		}
	}

}

int main(int argc, char *argv[]){

	init_sdl();
	char* path = "/Users/gustave/Documents/c/images/image_03.jpeg";
	char* savepath = "/Users/gustave/Documents/c/images/image_03grey.jpeg";

	SDL_Surface* image = load_image(path);
	greyscale(image);

	IMG_SavePNG(image, savepath);
}

