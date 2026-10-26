
#include "../include/admin.h"

static void admin_login_clicked(GtkWidget *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);

    GtkWidget *username_entry;
    GtkWidget *password_entry;

    username_entry = g_object_get_data(
        G_OBJECT(window), "username_entry"
    );

    password_entry = g_object_get_data(
        G_OBJECT(window), "password_entry"
    );

    const char *username = gtk_editable_get_text(
        GTK_EDITABLE(username_entry)
    );

    const char *password = gtk_editable_get_text(
        GTK_EDITABLE(password_entry)
    );

    if (g_strcmp0(username, "admin") == 0 &&
        g_strcmp0(password, "admin123") == 0)
    {
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new(
            window,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Admin Login Successful!"
        );

        g_signal_connect_swapped(
            dialog,
            "response",
            G_CALLBACK(gtk_window_destroy),
            dialog
        );

        gtk_window_present(GTK_WINDOW(dialog));
    }
    else
    {
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new(
            window,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Invalid username or password!"
        );

        g_signal_connect_swapped(
            dialog,
            "response",
            G_CALLBACK(gtk_window_destroy),
            dialog
        );

        gtk_window_present(GTK_WINDOW(dialog));
    }
}

void show_admin_login(GtkWindow *parent)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *username_label;
    GtkWidget *password_label;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *login_button;
    GtkWidget *back_button;

    window = gtk_window_new();

    gtk_window_set_title(GTK_WINDOW(window), "Admin Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(window), parent);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_widget_set_margin_top(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_margin_start(box, 35);
    gtk_widget_set_margin_end(box, 35);

    title = gtk_label_new("Admin Login");
    gtk_widget_add_css_class(title, "title-2");

    username_label = gtk_label_new("Username");
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(
        GTK_ENTRY(username_entry), "Enter username"
    );

    password_label = gtk_label_new("Password");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);

    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(
        GTK_ENTRY(password_entry), "Enter password"
    );
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    login_button = gtk_button_new_with_label("Login");
    back_button = gtk_button_new_with_label("Back");

    gtk_box_append(GTK_BOX(box), title);
    gtk_box_append(GTK_BOX(box), username_label);
    gtk_box_append(GTK_BOX(box), username_entry);
    gtk_box_append(GTK_BOX(box), password_label);
    gtk_box_append(GTK_BOX(box), password_entry);
    gtk_box_append(GTK_BOX(box), login_button);
    gtk_box_append(GTK_BOX(box), back_button);

    gtk_window_set_child(GTK_WINDOW(window), box);

    g_object_set_data(
        G_OBJECT(window), "username_entry", username_entry
    );

    g_object_set_data(
        G_OBJECT(window), "password_entry", password_entry
    );

    g_signal_connect(
        login_button,
        "clicked",
        G_CALLBACK(admin_login_clicked),
        window
    );

    g_signal_connect_swapped(
        back_button,
        "clicked",
        G_CALLBACK(gtk_window_destroy),
        window
    );

    gtk_window_present(GTK_WINDOW(window));
}
