#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <webkit/webkit.h>

void        focus_spy_callback      (GtkWindow *window, GtkWidget *widget, gpointer user_data)
{
    g_message( "wnd: %x  focus: %x\n", window,widget );
}

void
spy_callback(GObject *obj, GParamSpec *pspec, gpointer user_data)
{
    g_message( "obj: %x  prop: %s   f: %d\n", obj, pspec->name, gtk_widget_has_focus( GTK_WIDGET(obj) ) );
}

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
    paned = gtk_builder_get_object (builder, "paned2");
    alignment1 = gtk_builder_get_object (builder, "alignment1");
    alignment2 = gtk_builder_get_object (builder, "alignment2");

    gtk_window_set_focus_on_map( GTK_WINDOW(window), TRUE );
    gvim_socket = gtk_socket_new ();
    gtk_widget_set_can_focus (gvim_socket, TRUE);
    gtk_container_add (GTK_CONTAINER (alignment1), gvim_socket);
    webkit_view = webkit_web_view_new ();
    gtk_container_add (GTK_CONTAINER (alignment2), webkit_view);

    g_signal_connect( window, "set-focus", G_CALLBACK(focus_spy_callback), NULL );
    g_signal_connect( window, "notify", G_CALLBACK(spy_callback), NULL );
    g_signal_connect( G_OBJECT(gvim_socket), "notify", G_CALLBACK(spy_callback), NULL );
    g_signal_connect( G_OBJECT(webkit_view), "notify", G_CALLBACK(spy_callback), NULL );
    webkit_web_view_open (WEBKIT_WEB_VIEW (webkit_view), "http://ya.ru");
    spawn_gvim (gtk_socket_get_id (GTK_SOCKET (gvim_socket)));

    gtk_window_set_title (GTK_WINDOW (window), "ViIdE");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL); 
    gtk_widget_grab_focus (gvim_socket);
    gtk_widget_show_all (GTK_WIDGET (window)); 
    gtk_window_activate_focus( GTK_WINDOW(window) );
    gtk_widget_activate( gvim_socket );
    //gtk_paned_set_position (GTK_PANED (paned), gtk_widget_get_allocated_width (GTK_WIDGET (paned)) / 2);
    gtk_paned_set_position (GTK_PANED (paned), 700);

    gtk_main (); 
    return 0;
}
