// TODO
// Put a file explorer thing
// Finish the steps
// Header with css
// Brush my teeths
// Go to the toilets
// Change generated image path


#include <gtk/gtk.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <stdio.h>
#include "../include/rotate_img.h"
#include "../include/matrix.h"
#include "../include/pixel.h"
#include "../include/image_loading.h"
#include "../include/image_processing.h"
#include "../include/thresholding.h"
#include "../include/utils.h"
#include "../include/morph.h"
#include "../include/geometry.h"
#include "../include/edge_detection.h"
#include "../include/grid_detection.h"
#include "../include/grid_extraction.h"
// #include "../include/display.h"
#include "../include/NeuralNetwork.h"
#include "../include/solver.h"
#include "../include/funcs.h"
#include "../include/new_ai.h"
#include "../include/tools.h"
#include "../include/solvedGrid.h"

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
GtkFileChooserButton *chooser = NULL;
gchar* path = NULL;
Square blobtest;
SDL_Surface *homoT = NULL;

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
	/*sty = gtk_css_provider_new();
	gtk_css_provider_load_from_path(sty,"style.css",&error);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(sty),
                               GTK_STYLE_PROVIDER_PRIORITY_USER); */

	  /* Getting pointers for the different Items */
	window = GTK_WIDGET(gtk_builder_get_object (builder, "mainwindow"));
	impButton = GTK_WIDGET(gtk_builder_get_object(builder,"impButton"));
	image = GTK_WIDGET(gtk_builder_get_object(builder,"image"));
	fixedImg = GTK_WIDGET(gtk_builder_get_object(builder, "fixedImg"));
	infoLabel = GTK_LABEL(gtk_builder_get_object(builder,"infoLabel"));
	delButton = GTK_WIDGET(gtk_builder_get_object(builder, "delButton"));
	adju = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment"));
	rotscale = GTK_SCALE(gtk_builder_get_object(builder,"resizement"));
	chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder,"chooser"));
	  
 
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


void onChooser() 
{
	path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
}


// impButton Triggered
void on_impButton_clicked()
{
	if(path == NULL)
	{
		gtk_label_set_text(infoLabel,"[Error] You must select an image");
	}
	else
	{
		int widx = gtk_widget_get_allocated_width(fixedImg);
		int widy = gtk_widget_get_allocated_height(fixedImg);
	  	if (im2)
	  	{
				gtk_label_set_text(infoLabel, "[Warning] Image already imported");  
	  	}
	  	else
	  	{
			if(im1)
			{
				gtk_widget_hide(image);
				im1 = FALSE;
				im2 = TRUE;
				gtk_widget_destroy(image);
				//image2 = gtk_image_new_from_file("img/image.png");

				// GdkPixbuf* src = gdk_pixbuf_new_from_file(path,&error);
				// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
				GdkPixbuf *pixImage = gdk_pixbuf_new_from_file_at_scale(path,widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImage); 

				gtk_container_add(GTK_CONTAINER(fixedImg),image2);

				gtk_widget_show(image2);
				gtk_label_set_text(infoLabel,"[Action] Image Imported.");
			}
			else
			{
				im2 = TRUE;
				// GdkPixbuf* src = gdk_pixbuf_new_from_file(path,&error);
				// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);

				GdkPixbuf *pixImage = gdk_pixbuf_new_from_file_at_scale(path,widx,widy,TRUE,NULL);

				image2 = gtk_image_new_from_pixbuf(pixImage); 
				
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_widget_show(image2);
				gtk_label_set_text(infoLabel,"[Action] Image Imported.");
			}
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

		system("exec rm ../bin/.temp/cells/*.png");
		system("exec rm ../bin/.temp/*.png");


		gtk_label_set_text(infoLabel,"[Action] Image successfully deleted!");
		
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
	SDL_Surface *img = load_image(path);
	if(im1)
	{
		im1 = FALSE;
		im2 = TRUE;
		// gtk_widget_destroy(image);
		gtk_widget_hide(image);
		gtk_container_remove(GTK_CONTAINER(fixedImg),image);
		rotate_img(img,value);
		// rotateImg("img/image.png",value);
		// image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,NULL);

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



		rotate_img(img,value);
		// rotateImg("img/image.png",value);
		//image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,NULL);
		// GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
		// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
		image2 = gtk_image_new_from_pixbuf(test); 
		gtk_widget_show(image2);

		gtk_container_add(GTK_CONTAINER(fixedImg),image2);
	}
	//gtk_widget_hide(image);
	else
	{
		im1 = FALSE;
		im2 = TRUE;
		gtk_widget_hide(image);
		gtk_container_remove(GTK_CONTAINER(fixedImg),image);
		// rotate_img90(img,value);
		rotateImg(path,value);
		//image2 = gtk_image_new_from_file("image_rotated.png");

		GdkPixbuf* test = gdk_pixbuf_new_from_file_at_scale("image_rotated.png",widx,widy,TRUE,&error);
		// GdkPixbuf* src = gdk_pixbuf_new_from_file("image_rotated.png",&error);
		// GdkPixbuf *pixImage = gdk_pixbuf_scale_simple(src,widx,widy,GDK_INTERP_BILINEAR);
		image2 = gtk_image_new_from_pixbuf(test); 
		gtk_widget_show(image2);

		gtk_container_add(GTK_CONTAINER(fixedImg),image2);
		//gtk_box_pack_start(GTK_BOX(fixedImg),image2,FALSE,TRUE,0);
	}
	gtk_label_set_text(infoLabel,"[Action] Rotation Successful");
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
		gtk_label_set_text(infoLabel,"[Warning] You are already at the first step");
	}
}

void onForward() // Forward Button Clicked
{
	if(step < 12)
		step++;
	else
	{
		step = 12;	
		gtk_label_set_text(infoLabel,"[Warning] You are already at the last step");
	}

	// char applying[50] = "";
	// sprintf(applying,"%s%d%s","[Action] Applying step ", step, "...");
	// gtk_label_set_text(infoLabel,applying);

}

void onLast() // Last Button Clicked
{
	step = 12; //Last step

	// char applying[50] = "";
	// sprintf(applying,"%s%d%s","[Action] Applying step ", step, "...");
	// gtk_label_set_text(infoLabel,applying);
}


void changeS() // Will take the steps, and then change the images
{
	// Size of the Image Window
	int widx = gtk_widget_get_allocated_width(fixedImg);
	int widy = gtk_widget_get_allocated_height(fixedImg);

	// SDL Surface for Func Calls
	SDL_Surface *img = NULL;


	GdkPixbuf* pixImg = NULL;
	if (!im2)
	{
		gtk_label_set_text(infoLabel,"[Error] You must import an image first.");
		return;
	}

	printf("Current step is step n%d\n", step);
	switch(step)
	{
		case 0:
			gtk_widget_hide(image2);
			gtk_label_set_text(infoLabel,"[0] Raw Image");
			gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
			GdkPixbuf* pixImg = gdk_pixbuf_new_from_file_at_scale(path,widx,widy,TRUE,NULL);
			image2 = gtk_image_new_from_pixbuf(pixImg); 
			gtk_widget_show(image2);
			gtk_container_add(GTK_CONTAINER(fixedImg),image2);
			break;

		case 1:
			gtk_label_set_text(infoLabel,"[1] Applying Greyscale...");
			if (access("../bin/.temp/grayscale.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/grayscale.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[1] Greyscale Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image(path);
				greyscale(img);
                opening(img, 7);
				IMG_SavePNG(img,"../bin/.temp/grayscale.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/grayscale.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[1] Greyscale Applied!");
			}
			break;

		case 2:
			gtk_label_set_text(infoLabel,"[2] Applying Gaussian Blur...");
			if (access("../bin/.temp/blur.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/blur.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[2] Gaussian Blur Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_label_set_text(infoLabel,"[2] Applying Gaussian Blur...");
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/grayscale.png");
				gauss_blur(img, 17, -1);
				IMG_SavePNG(img,"../bin/.temp/blur.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/blur.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[2] Gaussian Blur Applied!");
			}
			break;

		case 3:
			gtk_label_set_text(infoLabel,"[3] Applying Threshold...");
			if (access("../bin/.temp/thresh.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/thresh.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[3] Threshold Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/blur.png");
				adaptive_gaussthresholding(img,13,3);
				IMG_SavePNG(img,"../bin/.temp/thresh.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/thresh.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[3] Threshold Applied!");
			}
			break;

		case 4:
			gtk_label_set_text(infoLabel,"[4] Applying Dilatation...");
			if (access("../bin/.temp/dilate.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/dilate.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[4] Dilatation Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/thresh.png");
				//dilate(img,3);
                closing(img, 5);
				IMG_SavePNG(img,"../bin/.temp/dilate.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/dilate.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[4] Dilatation Applied!");
			}
			break;

		case 5:
			gtk_label_set_text(infoLabel,"[5] Applying Blob Detection...");
			if (access("../bin/.temp/blob.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/blob.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[5] Blob Detection Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/dilate.png");
				SDL_Surface *blob = blob_detection(img);
				IMG_SavePNG(blob,"../bin/.temp/blob.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/blob.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[5] Blob Detection Applied!");
			}
			break;

		case 6:
			gtk_label_set_text(infoLabel,"[6] Applying Erosion...");
			if (access("../bin/.temp/erose.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/erose.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[6] Erosion Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/blob.png");
				erose(img,3);
				IMG_SavePNG(img,"../bin/.temp/erose.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/erose.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[6] Erosion Applied!");
			}
			break;

		case 7:
			gtk_label_set_text(infoLabel,"[7] Applying Hough...");
			if (access("../bin/.temp/lines.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/lines.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[7] Hough Applied!");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image("../bin/.temp/blob.png");

			    unsigned int width = img->w;
				unsigned int height = img->h;	

				// Hough:
				gtk_label_set_text(infoLabel,"[7] Applying Hough...");
			    int rows = sqrt(height * height + width * width);
				int angle_precision = 360;
			    int *hough = calloc(angle_precision * rows, sizeof(int));
				hough_lines(img, angle_precision, 1, hough);

				int hough_threshold = get_biggest_bin(hough, rows, angle_precision) * 0.7;
				Line *lines = calloc(angle_precision * rows, sizeof(Line));
				int line_nb = hough_filter(hough, rows, angle_precision, hough_threshold, lines);
				free(hough);

				// Draw Lines 
				for (int i = 0; i < line_nb; i++)
					drawred(img, &lines[i]);

				IMG_SavePNG(img,"../bin/.temp/lines.png");

				blobtest = get_blobs(lines, line_nb, (int)width, (int)height);
		
		
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/lines.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[7] Hough Applied!");
			}
			break;

		case 8:
			gtk_label_set_text(infoLabel,"[8] Applying Homographic Transform...");
			if (access("../bin/.temp/homoT.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/homoT.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[8] Homographic Transform Applied");
			}
			else
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				img = load_image(path);
				erose(img,3);
				homoT = homographic_Transform(img,blobtest);
				IMG_SavePNG(homoT,"../bin/.temp/homoT.png");
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/homoT.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg); 
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[8] Homographic Transform Applied");
			}
			break;

		case 9:
			gtk_label_set_text(infoLabel,"[9] Applying Cells Extraction...");
			if (access("../bin/.temp/cells/00.png", F_OK) == 0)	
			{
				gtk_widget_hide(image2);
				gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
				pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/homoT.png",widx,widy,TRUE,NULL);
				image2 = gtk_image_new_from_pixbuf(pixImg);
				gtk_widget_show(image2);
				gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[9] Cells Extraction Applied");
			}
			else
			{
				// gtk_widget_hide(image2);
				// gtk_container_remove(GTK_CONTAINER(fixedImg),image2);
				// img = load_image("../bin/.temp/homoT.png");

				Square homorect;
				TupleInt a = { 0, 0 };
				TupleInt b = { homoT->w - 1, 0 };
				TupleInt c = { homoT->w - 1, homoT->h - 1 };
				TupleInt d = { 0, homoT->h - 1 };
				homorect.NW = a;
				homorect.NE = b;
				homorect.SE = c;
				homorect.SW = d;
				extract_cells(&homorect, homoT, "../bin/.temp/cells/");


				// IMG_SavePNG(homoT,"../bin/.emp/extract_cells.png");
				// pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/homoT.png",widx,widy,TRUE,NULL);
				// image2 = gtk_image_new_from_pixbuf(pixImg); 
				// gtk_widget_show(image2);
				// gtk_container_add(GTK_CONTAINER(fixedImg),image2);
				gtk_label_set_text(infoLabel,"[9] Cells Extraction Applied!");
			}
			break;

		case 10:
			gtk_label_set_text(infoLabel,"[10] Initializing Neural Network...");
			if (access("Brain", F_OK) == 0)	
			{
				// char *saved_locale;
				// saved_locale = setlocale(LC_NUMERIC, "C");
				// /* do your strtod thing */
				// setlocale(LC_NUMERIC, saved_locale);
				NeuralNetwork(2,"../bin/.temp/cells/");
				printf("UwU\n");
			}
			else
			{
				NeuralNetwork(1,"../bin/.temp/cells/");
			}
			// gtk_widget_hide(image2);
			// gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
			// pixImg = gdk_pixbuf_new_from_file_at_scale("../bin/.temp/homoT.png",widx,widy,TRUE,NULL);
			// image2 = gtk_image_new_from_pixbuf(pixImg);
			// gtk_widget_show(image2);
			// gtk_container_add(GTK_CONTAINER(fixedImg),image2);
			gtk_label_set_text(infoLabel,"[10] Neural Network Initialized!");
			break;

		case 11:
			gtk_label_set_text(infoLabel,"[11] Solving the Sudoku...");
			if (access("grid.result", F_OK) != 0)	
			{
				Solver(2,"grid_00");
			}
			gtk_label_set_text(infoLabel,"[11] Sudoku Solved!");
			break;

		case 12:
            printf("Test");
			if (access("grid.result", F_OK))
            {
                printf("Sudoku is not Solvable!");
                gtk_label_set_text(infoLabel,"Sudoku is not Solvable");
            }
            printf("UwU2");
			display_grid("grid_00","grid.result");
			gtk_widget_hide(image2);
			gtk_container_remove(GTK_CONTAINER(fixedImg) , image2);
            pixImg = gdk_pixbuf_new_from_file_at_scale("grid_solved.png",widx,widy,TRUE,NULL);
            image2 = gtk_image_new_from_pixbuf(pixImg);
            gtk_widget_show(image2);
            gtk_container_add(GTK_CONTAINER(fixedImg),image2);
            gtk_label_set_text(infoLabel,"[12] Final Grid Constructed!");
			break;

		default:
			printf("Default Case\n");
	}
}
