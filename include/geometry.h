#ifndef GEOMETRY_H
#define GEOMETRY_H

//#include "utils.h"

typedef struct TupleInt{
	int x;
	int y;
} TupleInt;

typedef struct Line
{
	int rho;
	float theta;
} Line;

typedef struct Segment
{
	TupleInt pt1;
	TupleInt pt2;
} Segment;

typedef struct Square
{
	TupleInt NW;
	TupleInt SW;
	TupleInt NE;
	TupleInt SE;
} Square;

typedef struct DigitInfo
{
	short xmin;
	short xmax;
	short ymin;
	short ymax;
	int size;

} DigitInfo;

int polar_intersection(TupleInt *res, Line line1, Line line2);
int line_intersect(TupleInt *res, Line line1, Line line2,
	   	int width, int height);
void draw_line(SDL_Surface *surf, Line *line);
Segment get_segment(SDL_Surface *image, Line *line);
Segment *get_segments(SDL_Surface *image, Line *lines, int len);
int intersect(Segment s1, Segment s2);
void reorganize_square(Square *sq);
int is_square(Square *sq, float tolerance);
int square_len(Square *sq);
void plotLine(SDL_Surface *surf, Line *line, Uint32 color);
void drawred(SDL_Surface *surf, Line *line);
void draw_square(SDL_Surface *surf, Square *sq);

#endif
