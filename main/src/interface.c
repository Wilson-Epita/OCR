#include "header.h"

Image   *img = NULL;
GtkBuilder  *builder = NULL;

void  select_the_image(GtkFileChooser* load)
{
    char *path;
    FreeImage(img);
    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(load));

    img = image_load(path);

    if (img)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object
                                                    (builder, "ocr")), TRUE);
        gtk_image_set_from_file(GTK_IMAGE(gtk_builder_get_object(builder,
                                                     "main_image")), path);
    }


}

void extract_text()
{
    BinaryImage *bin = applyFilters(img);
    detectionCall(bin);
    gtk_text_buffer_set_text (GTK_TEXT_BUFFER(gtk_builder_get_object(builder,
                                         "extractedText")),outputstring, - 1) ;

    gtk_widget_show_all (GTK_WIDGET(gtk_builder_get_object(builder,
                                                           "text_window")));

    FreeBinary(bin);
}


void saving()
{
    char *filename = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object
                                                     (builder,"folder"))));
    char *path;
    if(!filename)
        path ="retrun.txt";
    else
    {
        sprintf(filename, "%s/return.txt", filename);
        path = filename;
    }
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(gtk_builder_get_object
                                                (builder, "extractedText"));

    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);

    char * text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

    FILE *file = fopen(path, "w");

    fprintf(file, "%s\n", text);

    fclose(file);
}

int interfaceCall(int    argc,char **argv)
{
    GtkWidget *fenetre_principale = NULL;
    GtkWidget *ocrbutton = NULL;
    GError *error = NULL;
    gchar *filename = NULL;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    filename =  g_build_filename ("ui.xml", NULL);

    /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (builder, filename, &error);
    g_free (filename);
    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    gtk_builder_connect_signals (builder, NULL);

    /* Récupération du pointeur de la fenêtre principale */
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder,
                                                        "main_window"));

    /* Désactivation des bouttons ocr et save. */
    ocrbutton = GTK_WIDGET(gtk_builder_get_object (builder, "ocr"));
    gtk_widget_set_sensitive(ocrbutton, FALSE);


    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (fenetre_principale);

    gtk_main();

    return 0;
}

int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r)
    {
        r = rmdir(path);
    }

    return r;
}
