#include <gtk/gtk.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "include/rotate_img.h"

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

/////////////////////////////

 
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
    adju = GTK_WIDGET(gtk_builder_get_object(builder, "adjustment"));
    rotscale = GTK_WIDGET(gtk_builder_get_object(builder,"resizement"));
      
 
    /* signal "destroy" for the window to close */
    g_signal_connect (G_OBJECT (window), "destroy",
        (GCallback)gtk_main_quit, NULL);
    gtk_builder_connect_signals(builder,NULL);
 
    /* Displaying the Main Window */
    gtk_widget_show(window);
    image2 = NULL;
 
 
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
        image2 = gtk_image_new_from_file("img/image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
        //gtk_fixed_move(GTK_FIXED(fixedImg),image2, 175, 10);
        gtk_label_set_text(infoLabel,"Image Imported.");
        printf("Import Button Clicked\n");
    }
    else
    {
        im2 = TRUE;
        image2 = gtk_image_new_from_file("img/image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
        //gtk_fixed_move(GTK_FIXED(fixedImg),image2, 175, 10);
        gtk_label_set_text(infoLabel,"Image Imported.");
        printf("Import Button Clicked\n");
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
        gtk_widget_destroy(image2);
        image = gtk_image_new_from_file("img/no-image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image);
        gtk_widget_show(image);
        gtk_label_set_text(infoLabel,"Image successfully deleted!");
        
    }
    else if (im1)
    {
        gtk_label_set_text(infoLabel,"[Error] Image already deleted");
    }
    else
    {
        image = gtk_image_new_from_file("img/no-image.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image);
        im1 = TRUE;
    }
}


void mResize()
{
    gdouble value = gtk_range_get_value(GTK_RANGE(rotscale));
    SDL_Surface *img = IMG_Load("img/image.png");
    if(im1)
    {
        gtk_widget_destroy(image);
        im1 = FALSE;
        im2 = TRUE;
        rotate_img90(img,(double)value);
        image2 = gtk_image_new_from_file("image_rotated.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    else if (im2)
    {
        gtk_widget_destroy(image2);
        rotate_img90(img,value);
        image2 = gtk_image_new_from_file("image_rotated.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    //gtk_widget_hide(image);
    else
    {
        gtk_widget_destroy(image);
        im2 = TRUE;
        rotate_img90(img,value);
        image2 = gtk_image_new_from_file("image_rotated.png");
        gtk_container_add(GTK_CONTAINER(fixedImg),image2);
        gtk_widget_show(image2);
    }
    gtk_label_set_text(infoLabel,"Rotation Successful");
}
