#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "pixel.h"
#include "edge_detection.h"
#include "image_processing.h"
#include "image_loading.h"
#include "matrix.h"
#include "utils.h"
#include "geometry.h"
#include "grid_detection.h"
#include "morph.h"
#include "thresholding.h"
#include <string.h>

int avg_size(Square *sq, int len)
{
	double sum = 0;
	for (int i = 0; i < len; i++)
		sum += sq[i].NE.x - sq[i].NW.x;
	return sum / len;
}

int find_digit(Uint8* arr, int size, int tolerance, TupleShort *s)
{
	TupleShort center = { size/2, size/2 };
	int acc = 0;
	int found = 0;
	int c = center.y * size + center.x;
	if (arr[c] == 255)
	{
		*s = center;
		return 1;
	}

	TupleShort p1 = center;
	TupleShort p2 = center;
	TupleShort p3 = center;
	TupleShort p4 = center;
	while (acc < tolerance && !found)
	{
		if (!found && p1.y - 1 >= 0)
		{
			p1.y--;
			if (arr[p1.y*size + p1.x] == 255)
			{
				found = 1;
				s->x = p1.x;
				s->y = p1.y;
			}
		}

		if (!found && p2.y + 1 < size)
		{
			p2.y++;
			if (arr[p2.y*size + p2.x] == 255)
			{
				found = 1;
				s->x = p2.x;
				s->y = p2.y;
			}
		}

		if (!found && p3.x + 1 < size)
		{
			p3.x++;
			if (arr[p3.y*size + p3.x] == 255)
			{
				found = 1;
				s->x = p3.x;
				s->y = p3.y;
			}
		}

		if (!found && p4.x - 1 >= 0)
		{
			p4.x--;
			if (arr[p4.y*size + p4.x] == 255)
			{
				found = 1;
				s->x = p4.x;
				s->y = p4.y;
			}
		}

		acc++;
	}

	if (!found)
		return 0;
	return 1;
}

/*
Span *cell_fillv2(Uint8* arr, int size, TupleShort seed)
{
	Span *s = malloc(sizeof(DigitInfo));
	digit->xmin = seed.x, digit->xmax = seed.x;
	digit->ymin = seed.y, digit->ymax = seed.y;
	digit->size = 0;
	Stack *s = NULL;
	s = stack_init(s);
	Uint8 grey = 120;
	stack_push(s, seed);
	while (!stack_isempty(s))
	{
		Span *e = { 0, 0, 0, 0

	}


}
*/

DigitInfo *cell_fill(Uint8* arr, int size, TupleShort seed)
{
	DigitInfo *digit = malloc(sizeof(DigitInfo));
	digit->xmin = seed.x, digit->xmax = seed.x;
	digit->ymin = seed.y, digit->ymax = seed.y;
	digit->size = 0;
	Stack *s = NULL;
	s = stack_init(s);
	Uint8 grey = 120;
	stack_push(s, seed);
	while (!stack_isempty(s))
	{
		TupleShort e = { 0, 0 };
		stack_pop(s, &e);
		digit->size++;

		if (e.y < digit->ymin)
			digit->ymin = e.y;
		else if (e.y > digit->ymax)
			digit->ymax = e.y;

		if (e.x < digit->xmin)
			digit->xmin = e.x;
		else if (e.x > digit->xmax)
			digit->xmax = e.x;

		size_t c = e.y * size + e.x;
		arr[c] = grey;

		//North
		if (e.y - 1 >= 0)
		{
			if (arr[c - size] == 255)
			{
				TupleShort tp = { e.x, e.y - 1 };
				stack_push(s, tp);
			}
		}

		//South
		if (e.y + 1 < size)
		{
			if (arr[c + size] == 255)
			{
				TupleShort tp = { e.x, e.y + 1 };
				stack_push(s, tp);
			}
		}

		//West
		if (e.x - 1 >= 0)
		{
			if (arr[c-1] == 255)
			{
				TupleShort tp = { e.x - 1, e.y };
				stack_push(s, tp);
			}
		}

		//East
		if (e.x + 1 < size)
		{
			if (arr[c+1] == 255)
			{
				TupleShort tp = { e.x + 1, e.y };
				stack_push(s, tp);
			}
		}
	}
	stack_free(s);
	return digit;
}

void restore_digit(Uint8 *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (arr[i] == 120)
			arr[i] = 255;
		else
			arr[i] = 0;
	}
}

SDL_Surface *normalize_digit(SDL_Surface *cell, DigitInfo *d)
{
	int xlen = d->xmax - d->xmin;
	int ylen = d->ymax - d->ymin;
	int xc = (int)d->xmin;
	int yc = (int)d->ymin;
	SDL_Rect rect = { xc, yc, xlen, ylen };

	SDL_Surface *crop_tmp =
			SDL_CreateRGBSurface(0, xlen, ylen, 32, 0, 0, 0, 0);
    SDL_Surface *crop =
			SDL_ConvertSurfaceFormat(crop_tmp, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(crop_tmp);

	SDL_BlitSurface(cell, &rect, crop, NULL);

	float coeff = (float)28 / ylen;
	xlen *= coeff;
	ylen = 28;
	SDL_Surface *stretch_tmp =
			SDL_CreateRGBSurface(0, xlen, ylen, 32, 0, 0, 0, 0);
    SDL_Surface *stretch =
			SDL_ConvertSurfaceFormat(stretch_tmp, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(stretch_tmp);

	SDL_BlitScaled(crop, NULL, stretch, NULL);

	int x0 = 28/2 - xlen/2;
	SDL_Rect rect2 = { x0, 0, xlen, ylen };

	SDL_Surface *res = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
	SDL_BlitSurface(stretch, NULL, res, &rect2);
	SDL_FreeSurface(stretch);

	return res;
}

void clean_cell(SDL_Surface *img)
{
	int size = img->w * img->h;
	Uint8 *arr = malloc(size * sizeof(Uint8));
	get_binarray(img, arr);

	TupleShort seed = { 0, 0 };
	int tolerance = 6 * img->h / 28;
	int success = find_digit(arr, img->w, tolerance, &seed);
	if (success)
	{
		DigitInfo *d = cell_fill(arr, img->h, seed);
		if ((float) d->size / (float) size < 0.03)
		{
			Uint32 color = SDL_MapRGB(img->format, 255, 255, 255);
			SDL_FillRect(img, NULL, color);
		}
		else
		{
			restore_digit(arr, size);

			binarr_to_surf(arr, img, size);
			SDL_Surface *norm = normalize_digit(img, d);
			invert(norm);
			*img = *norm;

		}
		/*
		restore_digit(arr, size);

		binarr_to_surf(arr, img, size);
		SDL_Surface *norm = normalize_digit(img, d);
		invert(norm);
		*img = *norm;
		*/
	}
	else
	{
		Uint32 color = SDL_MapRGB(img->format, 255, 255, 255);
		SDL_FillRect(img, NULL, color);
	}

	free(arr);
}

void extract_cells(Square *sq, SDL_Surface *image, char* path)
{
	int avgy = ((sq->SW.y - sq->NW.y) + (sq->SE.y - sq->NE.y)) / 2 / 9;
	int avgx = ((sq->NE.x - sq->NW.x) + (sq->SE.x - sq->SW.x)) / 2 / 9;
    int len = 0;
    if (avgy > avgx)
        len = avgy;
    else
        len = avgx;
	int counter = 0;
	int x0 = sq->NW.x;
	int y0 = sq->NW.y;

	for (int i = 0; i < 9; i++)
	{
		x0 = sq->NW.x;
		for (int j = 0; j < 9; j++)
		{
			char *name = malloc(3 * sizeof(char));
			name[2] = 0;
			if (counter < 10)
				name[0] = '0';
			else
			{
				int temp = counter / 10;
				name[0] = temp + '0';
			}
			name[1] = (counter % 10) + '0';

			SDL_Surface* crop_s =
				SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
			SDL_Surface* temp_s =
				SDL_CreateRGBSurface(0, len, len, 32, 0, 0, 0, 0);
    		SDL_Surface* crop =
				SDL_ConvertSurfaceFormat(crop_s, SDL_PIXELFORMAT_RGB888, 0);
    		SDL_Surface* temp =
				SDL_ConvertSurfaceFormat(temp_s, SDL_PIXELFORMAT_RGB888, 0);
			SDL_FreeSurface(crop_s);
			SDL_FreeSurface(temp_s);
			SDL_Rect rect = { x0, y0, len, len };

			SDL_BlitSurface(image, &rect, temp, NULL);
			gauss_blur1D(temp, 17, -1);
			adaptive_gaussthresholding(temp, 15, 6);
			closing(temp, 5);
			clean_cell(temp);
			SDL_BlitScaled(temp, NULL, crop, NULL);

			char *ext = ".png";
			char *pathres = calloc(50 ,sizeof(char));
			strcat(pathres, path);
			strcat(pathres, name);
			strcat(pathres, ext);
			IMG_SavePNG(crop, pathres);

			SDL_FreeSurface(crop);
			SDL_FreeSurface(temp);
			free(pathres);
			free(name);

			x0 += avgx;
			counter++;
		}

		y0 += avgy;
	}
}
