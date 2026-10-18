#include <gtk/gtk.h>
#include "gui.h"
#include "admin.h"

static void show_role_screen(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = GTK_WIDGET(data);

    GtkWidget *role_window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *student_button;
    GtkWidget *driver_button;
    GtkWidget *admin_button;

    role_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(role_window), "BusMate - Select Role");
    gtk_window_set_default_size(GTK_WINDOW(role_window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(role_window), GTK_WIN_POS_CENTER);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(box), 40);

    title = gtk_label_new("SELECT ROLE");

    student_button = gtk_button_new_with_label("Student");
    driver_button = gtk_button_new_with_label("Driver");
    admin_button = gtk_button_new_with_label("Admin");
    g_signal_connect(admin_button, "clicked",
                 G_CALLBACK(adminMenu), NULL);

    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box), student_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), driver_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), admin_button, FALSE, FALSE, 5);

    gtk_container_add(GTK_CONTAINER(role_window), box);

    gtk_widget_show_all(role_window);
}

void start_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *login_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "BusMate");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(box), 40);

    title = gtk_label_new("BUSMATE");
    login_button = gtk_button_new_with_label("Login");

    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 10);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(login_button, "clicked",
                     G_CALLBACK(show_role_screen), window);

    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}