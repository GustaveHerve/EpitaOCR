#include <gtk/gtk.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../include/rotate_img.h"
#include "../include/matrix.h"
#include "../include/pixel.h"
#include "../include/image_loading.h"

// Global Declarations : 
GtkWidget *window = NULL;
GtkLabel *infoLabel = NULL;
GtkWidget *impButton = NULL;
GtkWidget *image = NULL;
GtkWidget *image2 = NULL;
GtkWidget *fixedImg = NULL;
GtkBuilder *builder = NULL;
GtkWidget *delButton = NULL;
GError *error = NULL;
GtkAdjustment *adju = NULL;
GtkScale *rotscale = NULL;
bool im1 = TRUE;
bool im2 = FALSE;
//GtkAllocation *allocation = NULL;
GdkPixbuf *pixImage = NULL;

/////////////////////////////

int rotateImg(char* path, double angle)
{
        int              w, h;
        cairo_surface_t *ima;

        ima = cairo_image_surface_create_from_png (path);
        w = cairo_image_surface_get_width (ima);
        h = cairo_image_surface_get_height (ima);
        cairo_surface_t* tgt = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
        cairo_t *cr = cairo_create(tgt);
        cairo_translate(cr, w/2.0, h/2.0);
        cairo_rotate (cr, angle * (double)(M_PI/180));
        cairo_translate(cr, - w/2.0, - h/2.0);
        cairo_set_source_surface(cr, ima,  0, 0);
        cairo_paint(cr);
        cairo_surface_write_to_png(tgt,"image_rotated.png");
        cairo_destroy(cr);
}


int main(int argc, char *argv [])
{

    gchar *filename = NULL;
 
    /* Initialisation of GTK library. */
     
    gtk_init(&argc, &argv);
 
 
        /* Chargement du fichier test.glade. */
    builder = gtk_builder_new_from_file ("config.glade");
 
      /* Getting pointers for the different Items */
    window = GTK_WIDGET(gtk_builder_get_object (builder, "mainwindow"));
    impButton = GTK_WIDGET(gtk_builder_get_object(builder,"impButton"));
    image = GTK_WIDGET(gtk_builder_get_object(builder,"image"));
    //image = gtk_image_new_from_file ("myfile.png");
    fixedImg = GTK_WIDGET(gtk_builder_get_object(builder, "fixedImg"));
    infoLabel = GTK_LABEL(gtk_builder_get_object(builder,"infoLabel"));
    delButton = GTK_WIDGET(gtk_builder_get_object(builder, "delButton"));
    adju = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment"));
    rotscale = GTK_SCALE(gtk_builder_get_object(builder,"resizement"));
      
 
    /* signal "destroy" for the window to close */
    g_signal_connect (G_OBJECT (window), "destroy",
        (GCallback)gtk_main_quit, NULL);
    gtk_builder_connect_signals(builder,NULL);
 
    /* Displaying the Main Window */
    gtk_widget_show(window);
    image2 = NULL;
/*    GtkAllocation* allocation = NULL;
    gtk_widget_get_allocation(fixedImg,allocation);
    GError** error = NULL;
*/    
/*    int widx = gtk_widget_get_allocated_width(GTK_WIDGET(fixedImg));
    int widy = gtk_widget_get_allocated_height(GTK_WIDGET(fixedImg));
    GdkPixbuf* src = gdk_pixbuf_new_from_file("img/image.png",&error);
    GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
*/ 
 
    printf("Launched !\n");
 
    gtk_main();
 
    return 0;
}

// impButton Triggered
void on_impButton_clicked()
{
  if (im2)
  {
        gtk_label_set_text(infoLabel, "[Error 403] Image already loaded!");  
  }
  else
  {
    if(im2)
        gtk_label_set_text(infoLabel,"Image already imported");

    else if(im1)
    {
        gtk_widget_hide(image);
        im1 = FALSE;
        im2 = TRUE;
        gtk_widget_destroy(image);
        //image2 = gtk_image_new_from_file("img/image.png");

        int widx = gtk_widget_get_allocated_width(GTK_WIDGET(fixedImg));
        int widy = gtk_widget_get_allocated_height(GTK_WIDGET(fixedImg));
        GdkPixbuf* src = gdk_pixbuf_new_from_file("img/image.png",&error);
        GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
        image2 = gtk_image_new_from_pixbuf(pixImage); 

        gtk_container_add(GTK_CONTAINER(fixedImg),image2);

        gtk_widget_show(image2);
        //gtk_fixed_move(GTK_FIXED(fixedImg),image2, 175, 10);
        gtk_label_set_text(infoLabel,"Image Imported.");
    }
    else
    {
        im2 = TRUE;
/*        image2 = gtk_image_new_from_file("img/image.png");*/
        int widx = gtk_widget_get_allocated_width(GTK_WIDGET(fixedImg));
        int widy = gtk_widget_get_allocated_height(GTK_WIDGET(fixedImg));
        GdkPixbuf* src = gdk_pixbuf_new_from_file("img/image.png",&error);
        GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
        image2 = gtk_image_new_from_pixbuf(pixImage); 
        
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
        //gtk_fixed_move(GTK_FIXED(fixedImg),image2, 175, 10);
        gtk_label_set_text(infoLabel,"Image Imported.");
    }
  }
}

// delButton Triggered
void on_delButton_clicked()
{
    if(im2)
    {
        im1 = TRUE;
        im2 = FALSE;
        if(gtk_range_get_value(GTK_RANGE(rotscale)) != 0)
        {
            im2 = TRUE;
            im1 = FALSE;
            gtk_range_set_value(GTK_RANGE(rotscale),0);
            gtk_widget_destroy(image2);
            im1 = TRUE;
            im2 = FALSE;
        }
        else
            gtk_widget_destroy(image2);
        image = gtk_image_new_from_file("img/no-image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image);
        gtk_widget_show(image);
        gtk_label_set_text(infoLabel,"Image successfully deleted!");
        
    }
    else if (im1)
    {
        im2 = FALSE;
        gtk_label_set_text(infoLabel,"[Error] Image already deleted");
    }
    else
    {
        im2 = FALSE;
        im1 = TRUE;
        image = gtk_image_new_from_file("img/no-image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image);
    }
}


void mResize()
{
    gdouble value = gtk_range_get_value(GTK_RANGE(rotscale));
    SDL_Surface *img = load_image("img/image.png");
    if(im1)
    {
        im1 = FALSE;
        im2 = TRUE;
        gtk_widget_destroy(image);
        rotate_img90(img,(double)value);
//        rotateImg("img/image.png",value);
        /*image2 = gtk_image_new_from_file("image_rotated.png");*/

        int widx = gtk_widget_get_allocated_width(GTK_WIDGET(fixedImg));
        int widy = gtk_widget_get_allocated_height(GTK_WIDGET(fixedImg));
        GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
        GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
        image2 = gtk_image_new_from_pixbuf(pixImage); 


        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    else if (im2)
    {
        im1 = FALSE;
        im2 = TRUE;
        gtk_widget_hide(image2);
        gtk_widget_destroy(image2);
        rotate_img90(img,value);
        //image2 = gtk_image_new_from_file("image_rotated.png");
        //gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        //gtk_widget_show(image2);
        //rotateImg("img/image.png",value);
        //image2 = gtk_image_new_from_file("image_rotated.png");

        int widx = gtk_widget_get_allocated_width(fixedImg);
        int widy = gtk_widget_get_allocated_height(fixedImg);
        GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
        GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
        image2 = gtk_image_new_from_pixbuf(pixImage); 

        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    //gtk_widget_hide(image);
    else
    {
        im1 = FALSE;
        im2 = TRUE;
        gtk_widget_destroy(image);
        rotate_img90(img,value);
        //rotateImg("img/image.png",value);
        //image2 = gtk_image_new_from_file("image_rotated.png");

        int widx = gtk_widget_get_allocated_width(fixedImg);
        int widy = gtk_widget_get_allocated_height(fixedImg);
        GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
        GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
        image2 = gtk_image_new_from_pixbuf(pixImage); 

        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    gtk_label_set_text(infoLabel,"Rotation Successful");
}
