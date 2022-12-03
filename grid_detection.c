#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <err.h>
#include "include/pixel.h"
#include "include/utils.h"
#include "include/geometry.h"

//HOUGH TRANSFORM RELATED METHODS
void hough_init(int res[], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			res[i * cols + j] = 0;
		}
	}
}

void hough_lines(SDL_Surface* image, int angleNb, int step, int res[])
{
	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			Uint32 pixel = get_pixel(image, j, i);
			Uint8 r = 0, g = 0, b = 0;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);
			if (r > 100)
			{
				for (int k = 0; k < angleNb; k+=step)
				{
					float rad = k * M_PI / 180;
					float rho = j * cos(rad) + i * sin(rad);
					if (rho < 0)
					{
						int copy = k;
						copy = (k + 180) % 360;
						unsigned int rhoi = roundf(-rho);
						res[rhoi * angleNb + copy]++; 

					}

					else
					{
						unsigned int rhoi = roundf(rho);
						res[rhoi * angleNb + k]++; 
					}
				
				}
			}
		}
	}
}

void hough_lines_gradient(SDL_Surface* image, int angleNb, Uint8 *edges, 
		Uint8 *angles, int res[])
{
	unsigned int width = image->w;
	unsigned int height = image->h;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			int c = i*width + j;
			if (edges[c] == 255)
			{
				float rad = angles[c] * M_PI / 180;
				float rho = j*cos(rad) + i*sin(rad);
				if (rho < 0)
					continue;
				unsigned int rhoi = roundf(rho);
				res[rhoi * angleNb + angles[c]]++;

			}
		}
	}
}

void nhsuppr(int input[], int x, int y, int xlen, int ylen, TupleInt *size)
{
	for (int i = y - ylen/2; i <= y + ylen/2; i++)
	{
		if (i < 0 || i >= size->x)
			continue;
		for (int j = x - xlen/2; j <= x + xlen/2; j++)
		{
			if (j < 0 || j >= size->y)
				continue;
			input[i * 360 + j] = 0;
		}
	}
}

int hough_filter(int input[], int rows, int cols, int threshold, Line res[])
{
	int acc = 0;
	TupleInt size = {rows,cols};
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (input[i * cols + j] >= threshold){
				Line new = {i,j};
				res[acc] = new;
				acc++;
				nhsuppr(input, j, i, 30, 90, &size);
			}
		}
	}

	return acc;
}

TupleInt hough_filter_local(int input[], int rows, int cols, int threshold, 
		int step, int tol, Line hor[], Line ver[])
{
	int horacc = 0;
	int veracc = 0;

	for (int i = 0; i < rows; i += step)
	{
		for (int j = 0; j < cols; j += step)
		{
			int imax = i;
			int jmax = j;

			for (int k = 0; k < step; k++)
			{
				for (int l = 0; l < step; l++)
				{
					if (i+k < rows && j+l < cols)
					{
						if (input[(i+k)*cols + j+l] > input[imax*cols + jmax])
						{
							imax = i+k;
							jmax = j+l;
						}
					}
				}
			}

			if (input[imax * cols +  jmax] >= threshold)
			{
				Line new;
				new.theta = jmax;
				new.rho = imax;
				if (jmax <= 0 + tol || jmax >= 360 - tol)
				{
					ver[veracc] = new;
					veracc++;
				}
				else if ( (jmax >= 90 - tol && jmax <= 90 + tol )

						|| (jmax >=  270 - tol && jmax <= 270 + tol))
				{
					hor[horacc] = new;
					horacc++;
				}
			}
		}
	}

	TupleInt res;
	res.x = horacc;
	res.y = veracc;
	return res;
}



void line_distances(Line* lines, int len, int* res)
{
	if (res == NULL)
		errx(1, "line_distances: failed allocating memory ");
	for (int i = 1; i < len; i++)
		res[i-1] = lines[i].rho - lines[i-1].rho;
}

int merge_lines(Line* lines, int len, int thresh, Line* res)
{
	if (len <= 0)
		return -1;
	int curdist = 0;
	int index = 0;
	Line max = lines[0];

	for (int i = 1; i < len; i++)
	{
		curdist += lines[i].rho - lines[i-1].rho;
		if (curdist > thresh)
		{
			res[index] = max;
			index++;
			max = lines[i];
			curdist = 0;
			continue;
		}

		if (lines[i].rho > max.rho)
			max = lines[i];
	}

	if (max.rho - res[index-1].rho > thresh)
		res[index] = max;

	return index+1; 
	
}

int average_dist(int* dists, int len)
{
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += dists[i];
	}
	return sum / len;
}

int average_segdist(Line* lines, int len, SDL_Surface *surf)
{
	Segment temp;
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		temp = get_segment(surf, &lines[i]);
		int hor = temp.pt2.x - temp.pt1.x;
		int ver = temp.pt2.y - temp.pt1.y;
		sum += sqrt(hor * hor + ver*ver);
	}

	return sum / len;
}

int average_weight(Line* lines, int len, int *hough)
{
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		int c = 180*lines[i].rho + lines[i].theta;
		sum += hough[c];
	}

	return sum / len;
}

Square get_blobs(Line* lines, int len, int width, int height)
{
	Square grid = { 0, 0, 0, 0 };
	int maxlen = 0;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			if (lines[i].theta == lines[j].theta)
			   continue;

			TupleInt pt1 = {0,0};
			if (line_intersect(&pt1, lines[i], lines[j], width, height))
			{
				for (int k = 0; k < len; k++)
				{
					if (lines[j].theta == lines[k].theta || k == i)
			   			continue;

					TupleInt pt2 = {0,0};
					int condition = line_intersect(&pt2, lines[j], lines[k],
						   	width, height);

					if (condition)
					{
						for (int l = 0; l < len; l++)
						{
							if (lines[k].theta == lines[l].theta || l == j || l == i)
			   					continue;

							TupleInt pt3 = {0,0};
							condition = line_intersect(&pt3, lines[k], lines[l],
						   	width, height);

							if (condition)
							{
								TupleInt pt4 = {0,0};
								if (line_intersect(&pt4, lines[i], lines[l], width, height))
								{
									Square *res = malloc(sizeof(Square));
									res->NW = pt1;
									res->NE = pt2;
									res->SW = pt4;
									res->SE = pt3;

									int squaretest = is_square(res, 0.1);
									if (squaretest)
									{
										int tmp = square_len(res);
										if (tmp > maxlen)
										{
											grid = *res;
											maxlen = tmp;
											reorganize_square(&grid);
										}
									}
									free(res);
								}
							}
						}
					}
				}
			}
		}
	}
	return grid;
}

int get_grid_linesold(Line* lines, int len, int* dis, int tolerance, 
		Line* res, int *hough)
{
	int begin = 0;
	int end = 0;

	for (int i = 1; i < len -1 && end - begin+1 < 10; i++)
	{
		int delta = dis[i] - dis[i-1];
		if (delta < 0)
			delta = -delta;
		if (delta <= tolerance)
			end++;

		else
		{
			begin = i;
			i++;
			end = i;
		}

	}

	if (res == NULL)
		errx(2, "get_grid_lines: failed allocating memory");
	int index = 0;
	for (int i = begin; i <= end; i++)
	{
		res[index] = lines[i];
		index++;
	}
	res[index] = lines[end+1];
	index++;

	return index;

}

int get_grid_lines(Line* lines, int len, int* dis, Line* res, int *hough)
{
	int begin = 0;
	int end = 1;

	for (int i = 1; i < len -1 && end - begin + 1 < 10; i++)
	{
		float dis_dev = 0;
		if (dis[i-1] > dis[i])
			dis_dev = 1 - (float)dis[i] / (float)dis[i-1];
		else
			dis_dev = 1 - (float)dis[i-1] / (float)dis[i];
		if (dis_dev < 0.3)
		{
			float line_dev = 0;
			int c1 = 360 * lines[i].rho + (int)lines[i].theta;
			int c2 = 360 * lines[i-1].rho + (int)lines[i-1].theta;
			int val1 = hough[c1];
			int val2 = hough[c2];
			if (val1 > val2)
				line_dev = 1 - (float)val2/(float)val1;
			else
				line_dev = 1 - (float)val1/(float)val2;
			if (line_dev < 0.3)
				end++;
			
			else
			{
				begin = i;
				end = i;
			}

		}
		else
		{
			begin = i;
			end = i;
		}


	}

	if (res == NULL)
		errx(2, "get_grid_lines: failed allocating memory");
	int index = 0;
	while (end - begin + 1 > 10)
		end--;
	for (int i = begin; i <= end; i++)
	{
		res[index] = lines[i];
		index++;
	}
	
	return index;

}

int get_grid(Line* lines, int len, Line* res, int *hough)
{
	if (len < 2)
		return 0;
	int* gX = malloc(sizeof(int) * (len-1));
	line_distances(lines, len, gX);
	int n = get_grid_linesold(lines, len, gX, 13, res, hough);
	free(gX);
	return n;

}

int get_intersection_count(Segment s, Segment* segments, int len)
{
	int res = 0;
	for (int i = 0; i < len; i++)
	{
		if (intersect(s, segments[i]))
				res++;
	}

	return res;
}

Segment *get_grid_seg(Segment* xseg, Segment* yseg, TupleInt lens)
{
	Segment *grid = calloc(20, sizeof(Segment));

	int count = 0;
	for (int i = 0; i < lens.x && count < 10; i++)
	{
		int inter = get_intersection_count(xseg[i], yseg, lens.y);
		if (inter == 10)
		{
			grid[count] = xseg[i];
			count++;
		}
	}

	for (int i = 0; i < lens.y && count < 20; i++)
	{
		int inter = get_intersection_count(yseg[i], xseg, lens.x);
		if (inter == 10)
		{
			grid[count] = yseg[i];
			count++;
		}
	}

	return grid;
}

int flood_fill(Uint8* total, Uint8* copy, TupleShort *size, TupleShort seed)
{
	Stack *s = stack_init(s);
	Uint8 grey = 120;
	int res = 0;
	stack_push(s, seed);
	while (!stack_isempty(s))
	{
		TupleShort e = { 0, 0 };
		stack_pop(s, &e);
		res++;
		size_t c = e.y * size->x + e.x;
		total[c] = grey;
		copy[c] = grey;

		//North
		if (e.y - 1 >= 0)
		{
			if (copy[c - size->x] == 255)
			{
				TupleShort tp = { e.x, e.y - 1 };
				stack_push(s, tp);
			}
		}

		//South
		if (e.y + 1 < size->y)
		{
			if (copy[c + size->x] == 255)
			{
				TupleShort tp = { e.x, e.y + 1 };
				stack_push(s, tp);
			}
		}

		//West
		if (e.x - 1 >= 0)
		{
			if (copy[c-1] == 255)
			{
				TupleShort tp = { e.x - 1, e.y };
				stack_push(s, tp);
			}
		}

		//East
		if (e.x + 1 < size->x)
		{
			if (copy[c+1] == 255)
			{
				TupleShort tp = { e.x + 1, e.y };
				stack_push(s, tp);
			}
		}
	}
	stack_free(s);
	return res;
}

void retrieveblob(Uint8 *blobimg, TupleShort *size)
{
	for (size_t i = 0; i < size->y; i++)
	{
		for (size_t j = 0; j < size->x; j++)
		{
			int c = i * size->x + j;
			if (blobimg[c] != 120)
				blobimg[c] = 0;
			else
				blobimg[c] = 255;
		}
	}
}

SDL_Surface *blob_detection(SDL_Surface *img)
{
	int n = img->h * img->w;
	Uint8 *original = malloc(sizeof(Uint8) * n);
	get_binarray(img, original);

	Uint8 *totalarr = malloc(sizeof(Uint8) * n);
	binarraycpy(original, totalarr, n);

	Uint8 *resblob = malloc(sizeof(Uint8) * n);

	TupleShort size = { img->w, img->h };
	int max = 0;
	for (int i = 0; i < size.y; i++)
	{
		for (int j = 0; j < size.x; j++)
		{
			int c = i * size.y + j;
			if (totalarr[c] >= 250)
			{
				Uint8 *copy = malloc(sizeof(Uint8) * n);
				binarraycpy(original, copy, n);

				TupleShort start = { j, i };
				int tmp = flood_fill(totalarr, copy, &size, start);
				if (tmp > max)
				{
					binarraycpy(copy, resblob, n);
					max = tmp;
				}
				free(copy);
			}
		}
	}

	SDL_Surface *resimg = SDL_CreateRGBSurface(0, img->w, 
												img->h, 32, 0, 0, 0, 0);
 	resimg = SDL_ConvertSurfaceFormat(resimg, SDL_PIXELFORMAT_RGB888, 0);
	retrieveblob(resblob, &size);
	binarr_to_surf(resblob, resimg, n);
	free(totalarr);
	free(resblob);
	free(original);
	return resimg; 
}
