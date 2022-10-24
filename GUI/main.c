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
  /* Initialisation de la librairie Gtk. */
  gtk_init(&argc, &argv);

  /* Ouverture du fichier Glade de la fenêtre principale */

  /* Création du chemin complet pour accéder au fichier test.glade. */
  /* g_build_filename(); construit le chemin complet en fonction du système */
  /* d'exploitation. ( / pour Linux et \ pour Windows) */

      /* Chargement du fichier test.glade. */
  builder = gtk_builder_new_from_file ("config.glade");

    /* Récupération du pointeur de la fenêtre principale */
  window = GTK_WIDGET(gtk_builder_get_object (builder, "mainwindow"));
  impButton = GTK_WIDGET(gtk_builder_get_object(builder,"impButton"));
  image = GTK_WIDGET(gtk_builder_get_object(builder,"image"));
  image = gtk_image_new_from_file ("myfile.png");
  fixedImg = GTK_WIDGET(gtk_builder_get_object(builder, "fixedImg"));
  infoLabel = GTK_LABEL(gtk_builder_get_object(builder,"infoLabel"));
  delButton = GTK_WIDGET(gtk_builder_get_object(builder, "delButton"));

  /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
  /* fermeture de la fenêtre. */
  g_signal_connect (G_OBJECT (window), "destroy", (GCallback)gtk_main_quit, NULL);
  gtk_builder_connect_signals(builder,NULL);

  /* Affichage de la fenêtre principale. */
  gtk_widget_show(window);
  image2 = NULL;


  printf("Launched !\n");

  gtk_main();

  return 0;
}


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
  printf("UwU\n");
  gtk_label_set_text(infoLabel,"Image Deleted.");
  gtk_widget_hide(image2);
  image2 = NULL;
  gtk_widget_show(image);
}
