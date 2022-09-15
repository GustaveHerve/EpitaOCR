#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//get_pixel_ref: Get adress of the first byte composing the pixel at (x,y) coordinates
//				Not useful on its own but needed for get_pixel
Uint8* get_pixel_ref(SDL_Surface* image, unsigned x, unsigned y){
	return (Uint8*)image->pixels + y * image->pitch + 
		x * image->format->BytesPerPixel;
}

//get_pixel: Returns the (x,y) pixel data of surface image
Uint32 get_pixel(SDL_Surface *image, unsigned x, unsigned y){
	Uint8 *p = get_pixel_ref(image, x, y);

	switch (image->format->BytesPerPixel){

		case 1:	
			return *p;		//The pixel is composed of one byte so nothing to do here

		case 2:
			return *(Uint16*)p;		
			
		case 3:			//There is no Uint24 type, we need to return it on a Uint32 instead (1 byte won't be used)
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)	//BIG ENDIAN = Left byte is the most significant one
				return p[0] << 16 | p[1] << 8 | p[2];	
			else									//LITTLE ENDIAN = Left byte is the least signficant one
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;
	}
	return 0;
}

//replace_pixel : 
void replace_pixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel){
	Uint8 *p = get_pixel_ref(image, x, y);

	switch(image->format->BytesPerPixel){
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;
			
		case 3:		//& (AND) 0xff used to only keep 8 bit from the Uint32
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