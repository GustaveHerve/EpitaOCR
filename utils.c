#include <stdlib.h>
#include <SDL2/SDL.h>
#include "include/utils.h"
#include "include/pixel.h"
#include <err.h>

int max2(int a, int b)
{
    if (a < b)
        return b;
    return a;
}

int max3(int a, int b, int c)
{
    int temp = max2(a,b);
    if (temp < c)
        return c;
    return temp;
}

int min2(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

int min3(int a, int b, int c)
{
    int temp = min2(a,b);
    if (temp > c)
        return c;
    return temp;
}

//HIST

void histo_init(Histo* hist)
{
	for (unsigned int i = 0; i < 256; i++){
		hist->values[i] = 0;
	}
}

void histo_compute(SDL_Surface* image, Histo* hist)
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
			hist->values[r] += 1;

		}
	}
}

int get_biggest_bin(int* hough, int maxr, int maxtheta)
{
	int max = 0;
	for (int i = 0; i < maxr; i++)
	{
		for (int j = 0; j < maxtheta; j++)
		{
			int c = i * maxtheta + j;
			if (hough[c] > max)
				max = hough[c];
		}
	}
	return max;
}

float array_sum(float arr[], int begin, int end)
{
	float res = 0;
	for (int i = begin; i < end; i++)
	{
		res += arr[i];
	}

	return res;
}

int array_min_index(float arr[], size_t len)
{
	size_t min = 0;
	for (size_t i = 1; i < len; i++)
	{
		if (arr[i] < arr[min])
			min = i;
	}

	return (int) min;
}

//STACK

Stack_Tint* newStack_Tint(int size)
{
	Stack_Tint* res = malloc(sizeof(Stack_Tint));	
	res->maxsize = size;
	res->arr = malloc(size * sizeof(TupleInt));
	res->top = -1;
	return res;

}

int stackTint_is_empty(Stack_Tint* s)
{
	if (s->top == -1)
		return 1;
	return 0;
}

int stackTint_push(Stack_Tint* s, TupleInt e)
{
	if (s->top == s->maxsize - 1)
		return 0;

	s->top++;
	s->arr[s->top] = e;
	return 1;
}

TupleInt stackTint_pop(Stack_Tint* s)
{
	if (s->top == -1)
		errx(1, "Tried to pop empty stack");
	s->top--;
	return s->arr[s->top+1];
}

// SQUARE TUPLEINT
TupleInt minY2(TupleInt A, TupleInt B)
{
	if (A.y <= B.y)
		return A;
	return B;
}

TupleInt minY3(TupleInt A, TupleInt B, TupleInt C)
{
	TupleInt temp = minY2(A,B);
	if (temp.y <= C.y)
		return temp;
	return C;
}

TupleInt minY4(TupleInt A, TupleInt B, TupleInt C, TupleInt D)
{
	TupleInt temp = minY3(A, B, C);
	if (temp.y <= D.y)
		return temp;
	return D;
}
