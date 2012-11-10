#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <webkit/webkit.h>

void
spawn_gvim(Window socketid)
{
    gchar cmdline[256];

    g_snprintf (cmdline, sizeof (cmdline), "gvim --socketid 0x%x -u vimrc -U gvimrc", (unsigned int)socketid);
    g_spawn_command_line_sync (cmdline, NULL, NULL, NULL, NULL);
}

int
main(int argc, char **argv)
{
    GtkBuilder *builder;
    GObject *window;
    GObject *paned;
    GObject *alignment1, *alignment2;
    GtkWidget *gvim_socket;
    GtkWidget *webkit_view;

    gtk_init (&argc, &argv); 
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "viide.ui", NULL);
    window = gtk_builder_get_object (builder, "window");
    paned = gtk_builder_get_object (builder, "paned");
    alignment1 = gtk_builder_get_object (builder, "alignment1");
    alignment2 = gtk_builder_get_object (builder, "alignment2");

    gvim_socket = gtk_socket_new ();
    gtk_container_add (GTK_CONTAINER (alignment1), gvim_socket);
    webkit_view = webkit_web_view_new ();
    gtk_container_add (GTK_CONTAINER (alignment2), webkit_view);
    webkit_web_view_open (WEBKIT_WEB_VIEW (webkit_view), "http://ya.ru");

    spawn_gvim (gtk_socket_get_id (GTK_SOCKET (gvim_socket)));

    gtk_window_set_title (GTK_WINDOW (window), "ViIdE");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL); 
    gtk_widget_show_all (GTK_WIDGET (window)); 
    //g_message ("w: %d", gtk_widget_get_allocated_width (GTK_WIDGET (paned)));
    //gtk_paned_set_position (GTK_PANED (paned), gtk_widget_get_allocated_width (GTK_WIDGET (paned)) / 2);
    gtk_paned_set_position (GTK_PANED (paned), 500);

    gtk_main (); 
    return 0;
}
