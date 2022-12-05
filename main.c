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
    double mat[4] = {4,-5,4,9};
    double mat2[4] = {2, 3, 7, 12};
    double* res = malloc(sizeof(double)*4);
    mul(mat, mat2, 2, 2, 2, res);
    print_matrix("result mult: ", res, 2, 2);
    */


    /*
    double mat[4] = {4,-5,4,9};
    double* inverted = calloc(4, sizeof(double));
    inverted = inverseMat(mat, 2);
    print_matrix("mat", mat,2, 2);
    print_matrix("inverted", inverted, 2, 2);
    free(inverted);
    */


    //homographic transform main
    
    
    init_sdl();
    SDL_Surface *img = load_image(argv[1]);
    

    TupleInt p1 = {630,178};
    TupleInt p2 = {1368,695};
    TupleInt p3 = {850,1436};
    TupleInt p4 = {109,918};  //coos of the corners of image 5 grid.

    TupleInt p1_2 = {337, 208};
    TupleInt p2_2 = {1162, 208};
    TupleInt p3_2 = {1162, 1032};
    TupleInt p4_2 = {337, 1032};

    TupleInt p1_sd = {82, 104};
    TupleInt p2_sd = {405, 88};
    TupleInt p3_sd = {439, 392};
    TupleInt p4_sd = {52, 405};


    Square corners = {p1,p4,p2,p3};
    Square corners_2 = {p1_2, p4_2, p2_2, p3_2};
    Square corners_sd = {p1_sd, p4_sd, p2_sd, p3_sd};
    
    homographic_Transform(img, corners_sd);
    SDL_FreeSurface(img);

    open_img_window("homographic_image.png");
    IMG_Quit();
    SDL_Quit(); 
    
    


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
