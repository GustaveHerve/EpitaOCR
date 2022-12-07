#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <err.h>
#include "geometry.h"

int max2(int a, int b);
int max3(int a, int b, int c);
int min2(int a, int b);
int min3(int a, int b, int c);

typedef struct Histo{
	int values[256];
} Histo;

void histo_init(Histo* hist);
void histo_compute(SDL_Surface* image, Histo* hist);


/*
typedef struct TupleInt{
	int x;
	int y;
} TupleInt;
*/

typedef struct TupleShort{
	short x;
	short y;
} TupleShort;

typedef struct Stack_Tint{
	int maxsize;
	TupleInt* arr;
	int top;
} Stack_Tint;

Stack_Tint* newStack_Tint(int size);
int stackTint_is_empty(Stack_Tint* s);
int stackTint_push(Stack_Tint* s, TupleInt e);
TupleInt stackTint_pop(Stack_Tint* s);

float array_sum(float arr[], int begin, int end);
int array_min_index(float arr[], size_t len);

int get_biggest_bin(int* hough, int maxr, int maxtheta);

typedef struct Stack{
	int size;
	int capacity;
	TupleShort *data;
} Stack;

Stack *stack_init();
int stack_isempty(Stack* s);
void stack_free(Stack* s);
void stack_doublesize(Stack *s);
void stack_push(Stack *s, TupleShort x);
int stack_pop(Stack *s, TupleShort *x);

typedef struct Span 
{
	short x1;
	short x2;
	short y;
	short dy;
	short xmin;
	short xmax;
	short ymin;
	short ymax;
	int size;

} Span;
typedef struct StackS{
	int size;
	int capacity;
	Span *data;
} StackS;

StackS *stacks_init();
int stacks_isempty(StackS* s);
void stacks_free(StackS* s);
void stacks_doublesize(StackS *s);
void stacks_push(StackS *s, Span x);
int stacks_pop(StackS *s, Span *x);


#endif
