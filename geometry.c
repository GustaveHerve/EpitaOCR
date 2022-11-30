#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "include/geometry.h"
#include "include/pixel.h"

//polar_intersection: computes intersection point x,y of line1 and line2
//returns 1 if success, 0 if no intersection point exists
int polar_intersection(TupleInt *res, Line line1, Line line2)
{
	//Convert to radians
    float ang1 = line1.theta * M_PI / 180;
    float ang2 = line2.theta * M_PI / 180;
	
    float ct1 = cos(ang1);
    float st1 = sin(ang1);
    float ct2 = cos(ang2);
    float st2 = sin(ang2);

    float d = ct1 * st2 - st1*ct2;

    if (d!=0.0f)
	{
        res->x = (int)((st2*line1.rho-st1*line2.rho)/d);
        res->y = (int)((-ct2*line1.rho+ct1*line2.rho)/d);
    }
	//Both lines never intersect
	else
		return 0;

	return 1;
}

int line_intersect(TupleInt *res, Line line1, Line line2, int width, int height)
{
	polar_intersection(res, line1, line2);
	if (res->x < 0 || res->x >= width)
		return 0;
	if (res->y < 0 || res->y >= height)
		return 0;
	return 1;
}

int is_square(Square *sq, float tolerance)
{
	int tempx = sq->NE.x - sq->NW.x;
	int tempy = sq->NE.y - sq->NW.y;
	int a = sqrt(tempx * tempx + tempy * tempy);

	tempx = sq->SE.x - sq->NE.x;
	tempy = sq->SE.y - sq->NE.y;
	int b = sqrt(tempx * tempx + tempy * tempy);

	/*
	int tempx = sq->SW.x - sq->SE.x;
	int tempy = sq->SW.y - sq->SE.y;
	int c = sqrt(tempx * tempx + tempy * tempy);

	int tempx = sq->NW.x - sq->SW.x;
	int tempy = sq->NW.y - sq->SW.y;
	int d = sqrt(tempx * tempx + tempy * tempy);
	*/

	float deviation = 0;
	if (a > b)
		deviation = 1 - b/a;
	else
		deviation = 1 - a/b;

	if (deviation > tolerance)
		return 0;
	return 1;
}

void draw_line(SDL_Surface *surf, Line *line)
{
	SDL_LockSurface(surf);
	//Convert to rad theta
	float theta = line->theta * M_PI / 180;
	int r = line->rho;
	double sinA = sin(theta);
    double cosA = cos(theta);
	
	Uint32 *pixels = surf->pixels;
	SDL_PixelFormat* format = surf->format;
	int width = surf->w;
	int height = surf->h;

	Uint32 red = SDL_MapRGB(format, 255, 0, 0);

	if (sinA != 0.0)
	{
		for (int x = 0; x < width; x++)
		{
			int y = -cosA/sinA * x + r/sinA;
			if (y >= 0 && y < height)
				pixels[y * width + x] = red;
		}
	}
	//Vertical lines handling
	else if (r >= 0 && r < width)
	{
		int x = r;
		for (int y = 0; y < height; y++)
		{
			pixels[y * width + x] = red;
		}
	}

	SDL_UnlockSurface(surf);
}

Segment get_segment(SDL_Surface *image, Line *line)
{
	unsigned int width = image->w;
	size_t max = 0;
	size_t acc = 0;

	float cos_v = cos(line->theta * M_PI / 180);
	float sin_v = sin(line->theta * M_PI / 180);

	TupleInt first = {0,0};
	TupleInt last = {0,0};

	Segment res = {{0,0}, {0,0}};

	//avoid dividing by 0
	//if sin is 0, cos can't be 0
	if (line->theta != 0 && line->theta != 180)
	{
		int online = 0;
		for (size_t j = 0; j < width; j++)
		{
			int i = ((line->rho - j*cos_v) / sin_v);

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 val = 0;
			SDL_GetRGB(pixel, image->format, &val, &val, &val);

			if (val == 255)
			{
				acc++;
				if (!online)
				{
					first.x = j;
					first.y = i;
					online = 1;
				}

				else
				{
					last.x = j;
					last.y = i;
				}
			}

			if (val == 0)
			{
				if (online)
					online = 0;
				
				if (acc > max)
				{
					max = acc;
					res.pt1.x = first.x;
					res.pt1.y = first.y;
					res.pt2.x = last.x;
					res.pt2.y = last.y;

				}

				acc = 0;

			}

		}
	
	}

	else
	{
		int online = 0;
		for (size_t i = 0; i < width; i++)
		{
			int j = ((line->rho - i*sin_v) / cos_v);

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 val = 0;
			SDL_GetRGB(pixel, image->format, &val, &val, &val);

			if (val == 255)
			{
				acc++;
				if (!online)
				{
					first.x = j;
					first.y = i;
					online = 1;
				}

				else
				{
					last.x = j;
					last.y = i;
				}
			}

			if (val == 0)
			{
				if (online)
					online = 0;
				
				if (acc > max)
				{
					max = acc;
					res.pt1.x = first.x;
					res.pt1.y = first.y;
					res.pt2.x = last.x;
					res.pt2.y = last.y;

				}

				acc = 0;

			}
		}
	}
	
	return res;
}

Segment *get_segments(SDL_Surface *image, Line *lines, int len)
{
	Segment *res = malloc(sizeof(Segment) * len);
	for (int i = 0; i < len; i++)
	{
		Segment temp = get_segment(image, &lines[i]);
		res[i] = temp;
	}
	return res;
}

int intersect(Segment s1, Segment s2)
{
	if (s2.pt1.x > s1.pt2.x || s2.pt2.x < s1.pt1.x)
		return 0;
	if (s2.pt2.y < s1.pt1.y || s2.pt1.y > s1.pt2.y)
		return 0;
	return 1;
}
