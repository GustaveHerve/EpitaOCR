#include <gtk/gtk.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../include/rotate_img.h"
#include "../include/matrix.h"
#include "../include/pixel.h"
#include "../include/image_loading.h"
#include "../include/image_processing.h"
#include "../include/thresholding.h"

// Global Declarations : 
GtkWidget *window = NULL;
GtkLabel *infoLabel = NULL;
GtkWidget *impButton = NULL;
GtkWidget *image = NULL;
GtkWidget *image2 = NULL;
GtkWidget *fixedImg = NULL;
GtkBuilder *builder = NULL;
GtkCssProvider *sty = NULL;
GtkWidget *delButton = NULL;
GError *error = NULL;
GtkAdjustment *adju = NULL;
GtkScale *rotscale = NULL;
bool im1 = TRUE;
bool im2 = FALSE;
GdkPixbuf *pixImage = NULL;
int step = 0;

/////////////////////////////

int rotateImg(char* path, double angle)
{
		int              w, h;
		cairo_surface_t *ima;

		ima = cairo_image_surface_create_from_png (path);
		w = cairo_image_surface_get_width (ima);
		h = cairo_image_surface_get_height (ima);
		if(w > h)
			h = w;
		else
			w = h;
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
	builder = gtk_builder_new_from_file("config.glade");
	sty = gtk_css_provider_new();
	gtk_css_provider_load_from_path(sty,"style.css",&error);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(sty),
                               GTK_STYLE_PROVIDER_PRIORITY_USER); 

	  /* Getting pointers for the different Items */
	window = GTK_WIDGET(gtk_builder_get_object (builder, "mainwindow"));
	impButton = GTK_WIDGET(gtk_builder_get_object(builder,"impButton"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image"));
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

	printf("Launched !\n");
 
	gtk_main();
 
	return 0;
}

// impButton Triggered
void on_impButton_clicked()
{
	int widx = gtk_widget_get_allocated_width(fixedImg);
	int widy = gtk_widget_get_allocated_height(fixedImg);
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
		step = 0;
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
	int widx = gtk_widget_get_allocated_width(fixedImg);
	int widy = gtk_widget_get_allocated_height(fixedImg);
	gdouble value = gtk_range_get_value(GTK_RANGE(rotscale));
	SDL_Surface *img = load_image("img/image.png");
	if(im1)
	{
		im1 = FALSE;
		im2 = TRUE;
		// gtk_widget_destroy(image);
		gtk_widget_hide(image);
		gtk_container_remove(GTK_CONTAINER(fixedImg),image);
		rotate_img90(img,value);
		// rotateImg("img/image.png",value);
		// image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,&error);

		image2 = gtk_image_new_from_pixbuf(test); 
		gtk_widget_show(image2);


		gtk_container_add(GTK_CONTAINER(fixedImg),image2);
		// gtk_box_pack_start(GTK_BOX(fixedImg),image2,FALSE,FALSE,0);
	}
	else if (im2)
	{
		im1 = FALSE;
		im2 = TRUE;
		gtk_widget_hide(image2);
		// gtk_widget_destroy(image2);
		gtk_container_remove(GTK_CONTAINER(fixedImg),image2);



		rotate_img90(img,value);
		// rotateImg("img/image.png",value);
		//image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,&error);
		// GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
		// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
		image2 = gtk_image_new_from_pixbuf(test); 
		gtk_widget_show(image2);

		gtk_container_add(GTK_CONTAINER(fixedImg),image2);
	}
	//gtk_widget_hide(image);
	else
	{
		printf("Waht're u doin her\n");
		im1 = FALSE;
		im2 = TRUE;
		gtk_widget_hide(image);
		gtk_container_remove(GTK_CONTAINER(fixedImg),image);
		// rotate_img90(img,value);
		rotateImg("img/image.png",value);
		//image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,&error);
		// GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
		// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
		image2 = gtk_image_new_from_pixbuf(test); 
		gtk_widget_show(image2);

		gtk_container_add(GTK_CONTAINER(fixedImg),image2);
		//gtk_box_pack_start(GTK_BOX(fixedImg),image2,FALSE,TRUE,0);
	}
	gtk_label_set_text(infoLabel,"Rotation Successful");
}


void onFirst() // First button clicked
{
	step = 0;
}

void onRewind() // Rewind Button Clicked
{
	if(step > 0)
		step--;
	else
	{
		step = 0;
		gtk_label_set_text(infoLabel,"You are already at the first step");
	}
}

void onForward() // Forward Button Clicked
{
	if(step < 9)
		step++;
	else
	{
		step = 9;	
		gtk_label_set_text(infoLabel,"You are already at the first step");
	}

}

void onLast() // Last Button Clicked
{
	step = 9; //Last step
}


void changeS() // Will take the steps, and then change the images
{
	// Size of the Image Window
	int widx = gtk_widget_get_allocated_width(fixedImg);
	int widy = gtk_widget_get_allocated_height(fixedImg);

	// SDL Surface for Func Calls
	SDL_Surface *img = NULL;

	GdkPixbuf* pixImg = NULL;

	printf("Current step is step n%d\n", step);
	switch(step)
	{
		case 0:
			gtk_widget_hide(image2);
			gtk_label_set_text(infoLabel,"[0] Raw Image");
			gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
			GdkPixbuf* pixImg = gdk_pixbuf_new_from_file_at_scale("img/image.png",widx,widy,TRUE,NULL);
			image2 = gtk_image_new_from_pixbuf(pixImg); 
			gtk_widget_show(image2);
			gtk_container_add(GTK_CONTAINER(fixedImg),image2);
			break;

		case 1:
			gtk_widget_hide(image2);
			gtk_label_set_text(infoLabel,"[1] Applying Greyscale...");
			gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
			img = load_image("img/image.png");
			greyscale(img);
			IMG_SavePNG(img,"grayscale.png");
			pixImg = gdk_pixbuf_new_from_file_at_scale("grayscale.png",widx,widy,TRUE,NULL);
			image2 = gtk_image_new_from_pixbuf(pixImg); 
			gtk_widget_show(image2);
			gtk_container_add(GTK_CONTAINER(fixedImg),image2);
			break;

		case 2:
			gtk_widget_hide(image2);
			gtk_label_set_text(infoLabel,"[2] Applying Threshold...");
			gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
			img = load_image("img/image.png");
			adaptive_gaussthresholding(img,11,2);
			IMG_SavePNG(img,"gThresholding.png");
			pixImg = gdk_pixbuf_new_from_file_at_scale("gThresholding.png",widx,widy,TRUE,NULL);
			image2 = gtk_image_new_from_pixbuf(pixImg); 
			gtk_widget_show(image2);
			gtk_container_add(GTK_CONTAINER(fixedImg),image2);
			break;

		default:
			printf("Default Case\n");
	}
}