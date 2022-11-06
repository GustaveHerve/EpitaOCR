#include <math.h>
#include <SDL2/SDL.h>
#include "include/geometry.h"
#include "include/pixel.h"

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
		return 0;
	return 1;
}

Segment get_segment(SDL_Surface *image, Line *line){

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
	if (line->theta != 0 && line->theta != 180){
		int online = 0;
		for (size_t j = 0; j < width; j++){

			int i = ((line->rho - j*cos_v) / sin_v);

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 val = 0;
			SDL_GetRGB(pixel, image->format, &val, &val, &val);

			if (val == 255){
				acc++;
				if (!online){
					first.x = j;
					first.y = i;
					online = 1;
				}

				else{
					last.x = j;
					last.y = i;
				}
			}

			if (val == 0){
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

	else{

		int online = 0;
		for (size_t i = 0; i < width; i++){

			int j = ((line->rho - i*sin_v) / cos_v);

			Uint32 pixel = get_pixel(image, j, i);
			Uint8 val = 0;
			SDL_GetRGB(pixel, image->format, &val, &val, &val);

			if (val == 255){
				acc++;
				if (!online){
					first.x = j;
					first.y = i;
					online = 1;
				}

				else{
					last.x = j;
					last.y = i;
				}
			}

			if (val == 0){
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

