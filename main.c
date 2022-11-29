#include <stdio.h>
#include <err.h>
#include <errno.h>
#include "include/rotate_img.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/sdaile.h"
#include "include/image_loading.h"
#include "include/matrix.h"
#include "include/geometry.h"

int main (int argc, char* argv[])
{
    /*
    double *mat = malloc(sizeof(double)*18);
    for(int i = 0; i <= 15; i++)
    {
	double j = i;
        *(mat+i) = j+1;
    }
    *(mat+9) = 8;
    *(mat+14) = 0; */
    int mat[4] = {1,2,3,4};
    int *inverted = malloc(sizeof(double)*18);
    inverted = inverseMat(mat, inverted, 4);
    print_matrix("mat", (double*)mat, 4, 4);
    print_matrix("inverted", (double*)inverted, 4, 4);
    


    //homographic transform main
    
    /*
    init_sdl();
    SDL_Surface *img = load_image(argv[1]);
    TupleInt p1 = {630, 178};
    TupleInt p2 = {1368, 695};
    TupleInt p3 = {109,918};
    TupleInt p4 = {850, 1436};

    Square corners = {p1,p3, p2, p4};
    
    homographicT(img, corners);
    SDL_FreeSurface(img);

    open_img_window("homographic_image.png");
    IMG_Quit();
    SDL_Quit(); 
    */
    
    //manual rotation main
    /*
    init_sdl();
    SDL_Surface *img = load_image(argv[1]);
    double angle = atof(argv[2]);
    if(angle == 360)
    {
        open_img_window(argv[1]);
	return 0;
    }
    else
    {
        if (angle > 180)
		angle = -(360-angle);
//        if (angle == 90 || angle == 180)
            rotate_img90(img, angle);
//        else
//            rotate_img(img, angle); //atof converts a "string" to a double
    }
    open_img_window("image_rotated.png");
    SDL_FreeSurface(img);
    IMG_Quit();
    SDL_Quit();

    */

    return 0;
}
