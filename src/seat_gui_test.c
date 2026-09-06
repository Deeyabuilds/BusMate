#include <gtk/gtk.h>
#include <stdio.h>
#include "SeatManager.h"

static void on_seat_clicked(GtkWidget *widget, gpointer data)
{
    int seat_num = GPOINTER_TO_INT(data);

    if (book_seat_binary(seat_num, "Passenger"))
    {
        g_print("Seat %d booked successfully in binary file!\n", seat_num);
        gtk_button_set_label(GTK_BUTTON(widget), "Booked");
        gtk_widget_set_sensitive(widget, FALSE);
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "BusMate - Pure C Seat Selection");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_add(GTK_CONTAINER(window), grid);

    char label[10];
    int seatCounter = 1;
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            sprintf(label, "S%d", seatCounter);
            GtkWidget *button = gtk_button_new_with_label(label);

            if (is_seat_booked(seatCounter))
            {
                gtk_button_set_label(GTK_BUTTON(button), "Booked");
                gtk_widget_set_sensitive(button, FALSE);
            }

            g_signal_connect(button, "clicked", G_CALLBACK(on_seat_clicked), GINT_TO_POINTER(seatCounter));
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
            seatCounter++;
        }
    }

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    init_seats_file();

    GtkApplication *app = gtk_application_new("org.busmate.seatui", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}