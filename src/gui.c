#include <gtk/gtk.h>
#include "../include/SeatManager.h"
#include "../include/structures.h"
#include "../include/gui.h"

static void on_seat_clicked(GtkWidget *widget, gpointer data)
{
    int seat_num = GPOINTER_TO_INT(data);
    int current_bus_id = 101;   // Active Bus ID
    int current_student_id = 1; // Active Student ID

    if (is_seat_booked(seat_num, current_bus_id))
    {
        g_print("Seat %d is already booked.\n", seat_num);
        return;
    }

    if (book_seat_binary(seat_num, current_bus_id, current_student_id, "Window"))
    {
        g_print("Seat %d booked successfully for Student %d\n", seat_num, current_student_id);
        gtk_button_set_label(GTK_BUTTON(widget), "Booked");
        gtk_widget_set_sensitive(widget, FALSE);
    }
}

GtkWidget *create_seat_grid_view(void)
{
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    int current_bus_id = 101;

    for (int i = 1; i <= TOTAL_SEATS; i++)
    {
        char label[10];
        snprintf(label, sizeof(label), "%d", i);

        GtkWidget *btn = gtk_button_new_with_label(label);

        if (is_seat_booked(i, current_bus_id))
        {
            gtk_button_set_label(GTK_BUTTON(btn), "Booked");
            gtk_widget_set_sensitive(btn, FALSE);
        }
        else
        {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_seat_clicked), GINT_TO_POINTER(i));
        }

        int row = (i - 1) / SEATS_PER_ROW;
        int col = (i - 1) % SEATS_PER_ROW;
        gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
    }

    return grid;
}

void start_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BusMate - Seat Allocation");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = create_seat_grid_view();
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_widget_show_all(window);
    gtk_main();
}