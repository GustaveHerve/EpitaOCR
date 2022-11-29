#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

//update: Redraw texture on resize event on a renderer 
void update(SDL_Renderer *renderer, SDL_Texture *texture)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

//display_surface: display surf surface on debug window
void display_surface(SDL_Surface* surf)
{
	SDL_Window *window = SDL_CreateWindow("Sudoku OCR Debug Window",
										0,
										0,
										surf->w,
										surf->h,
										SDL_WINDOW_SHOWN |
										SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        errx(2, "display_surface: Couldn't create window");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetWindowSize(window, surf->w, surf->h);

    SDL_Event event;

	update(renderer, texture);
	int i = 1;

    while(i)
    {
        SDL_WaitEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
				i = 0;
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
					update(renderer, texture);
				break;

		}
    }

	SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
