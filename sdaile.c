#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
        printf("Invalid arguments");

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(1, "Failed to initialize SDL");    
    
    IMG_Init(IMG_INIT_JPG);
    
    SDL_Surface *img = IMG_Load(argv[1]);
    
    SDL_Window *window;

    window = SDL_CreateWindow("Cookin'VR",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,img->w/2,img->h/2,0);

    if(window == NULL)
        errx(1, "Failed to create window");
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    
    int i = 1;
    SDL_Event event;
    
    while(i)
    {
        SDL_WaitEvent(&event);
        
        if(event.type == SDL_QUIT)    
            i = 0;
        
        SDL_Rect img_size = {0,0,img->w/2, img->h/2};
        
        SDL_RenderCopy(renderer, texture, NULL, &img_size);
        //copy texture to output device
        
        SDL_RenderPresent(renderer); 
        //it sends the renderer to window

    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(img);
    IMG_Quit();
    SDL_Quit();

    return 0;
}