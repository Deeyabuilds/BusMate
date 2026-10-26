
#include "../include/gui.h"

static void show_message(GtkWindow *parent, const char *message)
{
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s",
        message
    );

    g_signal_connect_swapped(
        dialog,
        "response",
        G_CALLBACK(gtk_window_destroy),
        dialog
    );

    gtk_window_present(GTK_WINDOW(dialog));
}

/* Admin section */
static void admin_login(GtkWidget *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);

    show_admin_login(window);
}

/* Student section */
static void student_login(GtkWidget *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);

    show_message(window, "Student Login");
}

/* Driver section */
static void driver_login(GtkWidget *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);

    show_message(window, "Driver Login");
}

/* Exit application */
static void exit_app(GtkWidget *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);

    gtk_window_destroy(window);
}

/* Main home screen */
void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *subtitle;
    GtkWidget *admin_button;
    GtkWidget *student_button;
    GtkWidget *driver_button;
    GtkWidget *exit_button;

    window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "BusMate");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);

    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    title = gtk_label_new("BusMate");
    gtk_widget_add_css_class(title, "title-1");

    subtitle = gtk_label_new(
        "Smart College Bus Management System"
    );

    admin_button = gtk_button_new_with_label("Admin Login");
    student_button = gtk_button_new_with_label("Student Login");
    driver_button = gtk_button_new_with_label("Driver Login");
    exit_button = gtk_button_new_with_label("Exit");

    gtk_widget_set_size_request(admin_button, 220, 45);
    gtk_widget_set_size_request(student_button, 220, 45);
    gtk_widget_set_size_request(driver_button, 220, 45);
    gtk_widget_set_size_request(exit_button, 220, 45);

    gtk_box_append(GTK_BOX(box), title);
    gtk_box_append(GTK_BOX(box), subtitle);
    gtk_box_append(GTK_BOX(box), admin_button);
    gtk_box_append(GTK_BOX(box), student_button);
    gtk_box_append(GTK_BOX(box), driver_button);
    gtk_box_append(GTK_BOX(box), exit_button);

    gtk_window_set_child(GTK_WINDOW(window), box);

    g_signal_connect(
        admin_button, "clicked",
        G_CALLBACK(admin_login), window
    );

    g_signal_connect(
        student_button, "clicked",
        G_CALLBACK(student_login), window
    );

    g_signal_connect(
        driver_button, "clicked",
        G_CALLBACK(driver_login), window
    );

    g_signal_connect(
        exit_button, "clicked",
        G_CALLBACK(exit_app), window
    );

    gtk_window_present(GTK_WINDOW(window));
}

