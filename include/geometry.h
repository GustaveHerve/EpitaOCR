#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "utils.h"

typedef struct Line{
	int rho;
	float theta;
} Line;

typedef struct Segment{
	TupleInt pt1;
	TupleInt pt2;
} Segment;

typedef struct Square{
	TupleInt NW;
	TupleInt SW;
	TupleInt NE;
	TupleInt SE;
} Square;


int polar_intersection(TupleInt *res, Line line1, Line line2);
Segment get_segment(SDL_Surface *image, Line *line);
Segment *get_segments(SDL_Surface *image, Line *lines, int len);
int intersect(Segment s1, Segment s2);

#endif
