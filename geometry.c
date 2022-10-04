#include <math.h>
#include "include/geometry.h"

//polar_intersection: computes intersection point x,y of line1 and line2
//returns 1 if success, 0 if no intersection point exists
int polar_intersection(TupleInt *res, Line line1, Line line2){

	//Convert to radians
    float ang1 = line1.theta * M_PI / 180;
    float ang2 = line2.theta * M_PI / 180;
	
    float ct1 = cos(ang1);
    float st1 = sin(ang1);
    float ct2 = cos(ang2);
    float st2 = sin(ang2);

    float d = ct1 * st2 - st1*ct2;

    if (d!=0.0f){
        res->x = (int)((st2*line1.rho-st1*line2.rho)/d);
        res->y = (int)((-ct2*line1.rho+ct1*line2.rho)/d);
    }
	//Both lines never intersect
	else
		return 1;
	return 0;
}
