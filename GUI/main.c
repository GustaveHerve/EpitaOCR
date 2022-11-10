#include <gtk/gtk.h>
#include <stdio.h>

// Global declaration : 
GtkWidget *window = NULL;
GtkLabel *infoLabel = NULL;
GtkWidget *impButton = NULL;
GtkWidget *image = NULL;
GtkWidget *image2 = NULL;
GtkWidget *fixedImg = NULL;
GtkBuilder *builder = NULL;
GtkWidget *delButton = NULL;
GError *error = NULL;

/////////////////////////////

 
int main(int argc, char *argv [])
{
  gchar *filename = NULL;
  /* Initialising GTK */
  gtk_init(&argc, &argv);

  /* Ouverture du fichier Glade de la fenêtre principale */


      /* Loading the Glade config */
  builder = gtk_builder_new_from_file ("config.glade");

    /* Getting Widgets pointers */
  window = GTK_WIDGET(gtk_builder_get_object (builder, "mainwindow"));
  impButton = GTK_WIDGET(gtk_builder_get_object(builder,"impButton"));
  image = GTK_WIDGET(gtk_builder_get_object(builder,"image"));
  image = gtk_image_new_from_file ("myfile.png");
  fixedImg = GTK_WIDGET(gtk_builder_get_object(builder, "fixedImg"));
  infoLabel = GTK_LABEL(gtk_builder_get_object(builder,"infoLabel"));
  delButton = GTK_WIDGET(gtk_builder_get_object(builder, "delButton"));

  /* Destroy signal to close the Window */
  g_signal_connect (G_OBJECT (window), "destroy", (GCallback)gtk_main_quit, NULL);
  gtk_builder_connect_signals(builder,NULL);

  /* Display the Main Window. */
  gtk_widget_show(window);
  image2 = NULL;


  printf("Launched !\n");

  gtk_main();

  return 0;
}

// Check if buttons clicked

void on_impButton_clicked()
{
  if (image2)
  {
    gtk_label_set_text(infoLabel, "[Error 403] Image already loaded!");  
  }
  else
  {
    gtk_widget_hide(image);
    image2 = gtk_image_new_from_file("image.jpg");
    gtk_widget_show(image2);
    gtk_container_add(GTK_CONTAINER(fixedImg),image2);
    gtk_fixed_move(GTK_FIXED(fixedImg),image2, 175, 10);
    gtk_label_set_text(infoLabel,"Image Imported.");
    printf("Import Button Clicked\n");
  }
}

void on_delButton_clicked()
{
  printf("Delete Button Clicked\n");
  gtk_label_set_text(infoLabel,"Image Deleted.");
  gtk_widget_hide(image2);
  image2 = NULL;
  gtk_widget_show(image);
}
