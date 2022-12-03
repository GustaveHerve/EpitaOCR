#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <err.h>

int max2(int a, int b);
int max3(int a, int b, int c);
int min2(int a, int b);
int min3(int a, int b, int c);

typedef struct Histo{
	int values[256];
} Histo;

void histo_init(Histo* hist);
void histo_compute(SDL_Surface* image, Histo* hist);

typedef struct TupleInt{
	int x;
	int y;
} TupleInt;

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
	TupleInt *data;
} Stack;

Stack *stack_init();
int stack_isempty(Stack* s);
void stack_free(Stack* s);
void stack_doublesize(Stack *s);
void stack_push(Stack *s, TupleInt x);
int stack_pop(Stack *s, TupleInt *x);

#endif
