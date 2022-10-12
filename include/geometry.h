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

int polar_intersection(TupleInt *res, Line line1, Line line2);
Segment get_segment(SDL_Surface *image, Line *line);

#endif
