#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "utils.h"

typedef struct Line{
	int rho;
	float theta;
} Line;

int polar_intersection(TupleInt *res, Line line1, Line line2);

#endif
