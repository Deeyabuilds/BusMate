#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "member2_gui.h"
#include "bus.h"
#include "route.h"
#include "driver.h"
#include "schedule.h"

/* Helper to show an alert dialog */
static void show_alert(GtkWindow *parent, GtkMessageType type, const char *title, const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        type,
        GTK_BUTTONS_OK,
        "%s",
        message
    );
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/* ================= 1. BUS MANAGEMENT ================= */

static void add_bus_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog;
    GtkWidget *content;
    GtkWidget *grid;

    GtkWidget *id_entry;
    GtkWidget *number_entry;
    GtkWidget *capacity_entry;

    GtkWidget *id_label;
    GtkWidget *number_label;
    GtkWidget *capacity_label;

    dialog = gtk_dialog_new_with_buttons(
        "Add Bus",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Save Bus",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 380, 200);

    content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    id_label = gtk_label_new("Bus ID (numeric):");
    number_label = gtk_label_new("Bus Number:");
    capacity_label = gtk_label_new("Capacity (seats):");

    id_entry = gtk_entry_new();
    number_entry = gtk_entry_new();
    capacity_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), number_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), number_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), capacity_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), capacity_entry, 1, 2, 1, 1);

    gtk_container_add(GTK_CONTAINER(content), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int busId = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        int capacity = atoi(gtk_entry_get_text(GTK_ENTRY(capacity_entry)));
        const char *busNumber = gtk_entry_get_text(GTK_ENTRY(number_entry));

        if (busId <= 0 || capacity <= 0 || strlen(busNumber) == 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid Input", "Please provide valid ID, Bus Number, and Capacity.");
        }
        else
        {
            int res = addBus(busId, busNumber, capacity);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Bus added and saved successfully!");
            }
            else if (res == -2)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_WARNING, "Duplicate ID", "A bus with this ID already exists.");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Error", "Could not add bus. Limit reached or file error.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void view_buses_gui(GtkWidget *widget, gpointer data)
{
    loadBuses();

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "View Buses",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 350);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(text_view), 10);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    char text[4096] = "";
    if (busCount == 0)
    {
        snprintf(text, sizeof(text), "No bus records found.\n");
    }
    else
    {
        char header[256];
        snprintf(header, sizeof(header), "%-10s | %-20s | %-12s | %-15s\n", "BUS ID", "BUS NUMBER", "CAPACITY", "AVAILABLE");
        strncat(text, header, sizeof(text) - strlen(text) - 1);
        strncat(text, "----------------------------------------------------------------------\n", sizeof(text) - strlen(text) - 1);

        for (int i = 0; i < busCount; i++)
        {
            char line[256];
            snprintf(line, sizeof(line), "%-10d | %-20s | %-12d | %-15d\n",
                     buses[i].busId, buses[i].busNumber, buses[i].capacity, buses[i].availableSeats);
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
    }

    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    gtk_container_add(GTK_CONTAINER(content), scrolled);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void remove_bus_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remove Bus",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Remove",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 15);

    GtkWidget *label = gtk_label_new("Enter Bus ID to remove:");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(content), box);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int busId = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
        if (busId <= 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid ID", "Please enter a valid positive Bus ID.");
        }
        else
        {
            int res = removeBus(busId);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Bus removed successfully!");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Not Found", "Bus ID not found.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void bus_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Bus Management",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 350, 220);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);

    GtkWidget *add_btn = gtk_button_new_with_label("➕  Add Bus");
    GtkWidget *view_btn = gtk_button_new_with_label("📋  View Buses");
    GtkWidget *rem_btn = gtk_button_new_with_label("🗑️  Remove Bus");

    gtk_widget_set_size_request(add_btn, -1, 38);
    gtk_widget_set_size_request(view_btn, -1, 38);
    gtk_widget_set_size_request(rem_btn, -1, 38);

    gtk_box_pack_start(GTK_BOX(box), add_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), view_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), rem_btn, FALSE, FALSE, 4);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_bus_gui), dialog);
    g_signal_connect(view_btn, "clicked", G_CALLBACK(view_buses_gui), dialog);
    g_signal_connect(rem_btn, "clicked", G_CALLBACK(remove_bus_gui), dialog);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


/* ================= 2. ROUTE MANAGEMENT ================= */

static void add_route_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Add Route",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Save Route",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 420, 240);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    GtkWidget *id_label = gtk_label_new("Route ID (numeric):");
    GtkWidget *name_label = gtk_label_new("Route Name:");
    GtkWidget *stops_label = gtk_label_new("Stops (comma-separated):");

    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *stops_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stops_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stops_entry, 1, 2, 1, 1);

    gtk_container_add(GTK_CONTAINER(content), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int routeId = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char *stops = gtk_entry_get_text(GTK_ENTRY(stops_entry));

        if (routeId <= 0 || strlen(name) == 0 || strlen(stops) == 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid Input", "Please provide valid Route ID, Name, and Stops.");
        }
        else
        {
            int res = addRoute(routeId, name, stops);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Route added and saved successfully!");
            }
            else if (res == -2)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_WARNING, "Duplicate ID", "A route with this ID already exists.");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Error", "Could not add route. Limit reached or file error.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void view_routes_gui(GtkWidget *widget, gpointer data)
{
    loadRoutes();

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "View Routes",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 550, 350);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(text_view), 10);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    char text[8192] = "";
    if (routeCount == 0)
    {
        snprintf(text, sizeof(text), "No route records found.\n");
    }
    else
    {
        char header[256];
        snprintf(header, sizeof(header), "%-10s | %-25s | %s\n", "ROUTE ID", "ROUTE NAME", "STOPS");
        strncat(text, header, sizeof(text) - strlen(text) - 1);
        strncat(text, "--------------------------------------------------------------------------------\n", sizeof(text) - strlen(text) - 1);

        for (int i = 0; i < routeCount; i++)
        {
            char line[512];
            snprintf(line, sizeof(line), "%-10d | %-25s | %s\n",
                     routes[i].routeId, routes[i].routeName, routes[i].stops);
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
    }

    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    gtk_container_add(GTK_CONTAINER(content), scrolled);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void remove_route_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remove Route",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Remove",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 15);

    GtkWidget *label = gtk_label_new("Enter Route ID to remove:");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(content), box);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int routeId = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
        if (routeId <= 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid ID", "Please enter a valid positive Route ID.");
        }
        else
        {
            int res = removeRoute(routeId);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Route removed successfully!");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Not Found", "Route ID not found.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void route_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Route Management",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 350, 220);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);

    GtkWidget *add_btn = gtk_button_new_with_label("➕  Add Route");
    GtkWidget *view_btn = gtk_button_new_with_label("🗺️  View Routes");
    GtkWidget *rem_btn = gtk_button_new_with_label("🗑️  Remove Route");

    gtk_widget_set_size_request(add_btn, -1, 38);
    gtk_widget_set_size_request(view_btn, -1, 38);
    gtk_widget_set_size_request(rem_btn, -1, 38);

    gtk_box_pack_start(GTK_BOX(box), add_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), view_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), rem_btn, FALSE, FALSE, 4);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_route_gui), dialog);
    g_signal_connect(view_btn, "clicked", G_CALLBACK(view_routes_gui), dialog);
    g_signal_connect(rem_btn, "clicked", G_CALLBACK(remove_route_gui), dialog);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


/* ================= 3. DRIVER MANAGEMENT ================= */

static void add_driver_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Add Driver",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Save Driver",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 380, 200);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    GtkWidget *id_label = gtk_label_new("Driver ID (numeric):");
    GtkWidget *name_label = gtk_label_new("Driver Name:");
    GtkWidget *phone_label = gtk_label_new("Phone Number:");

    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *phone_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), phone_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), phone_entry, 1, 2, 1, 1);

    gtk_container_add(GTK_CONTAINER(content), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int driverId = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const char *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));

        if (driverId <= 0 || strlen(name) == 0 || strlen(phone) == 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid Input", "Please provide valid Driver ID, Name, and Phone.");
        }
        else
        {
            int res = addDriver(driverId, name, phone);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Driver added and saved successfully!");
            }
            else if (res == -2)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_WARNING, "Duplicate ID", "A driver with this ID already exists.");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Error", "Could not add driver. Limit reached or file error.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void view_drivers_gui(GtkWidget *widget, gpointer data)
{
    loadDrivers();

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "View Drivers",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 350);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(text_view), 10);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    char text[4096] = "";
    if (driverCount == 0)
    {
        snprintf(text, sizeof(text), "No driver records found.\n");
    }
    else
    {
        char header[256];
        snprintf(header, sizeof(header), "%-12s | %-25s | %-18s\n", "DRIVER ID", "NAME", "PHONE");
        strncat(text, header, sizeof(text) - strlen(text) - 1);
        strncat(text, "------------------------------------------------------------\n", sizeof(text) - strlen(text) - 1);

        for (int i = 0; i < driverCount; i++)
        {
            char line[256];
            snprintf(line, sizeof(line), "%-12d | %-25s | %-18s\n",
                     drivers[i].driverId, drivers[i].name, drivers[i].phone);
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
    }

    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    gtk_container_add(GTK_CONTAINER(content), scrolled);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void remove_driver_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remove Driver",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Remove",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 15);

    GtkWidget *label = gtk_label_new("Enter Driver ID to remove:");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(content), box);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int driverId = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
        if (driverId <= 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid ID", "Please enter a valid positive Driver ID.");
        }
        else
        {
            int res = removeDriver(driverId);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Driver removed successfully!");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Not Found", "Driver ID not found.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void driver_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Driver Management",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 350, 220);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);

    GtkWidget *add_btn = gtk_button_new_with_label("➕  Add Driver");
    GtkWidget *view_btn = gtk_button_new_with_label("👤  View Drivers");
    GtkWidget *rem_btn = gtk_button_new_with_label("🗑️  Remove Driver");

    gtk_widget_set_size_request(add_btn, -1, 38);
    gtk_widget_set_size_request(view_btn, -1, 38);
    gtk_widget_set_size_request(rem_btn, -1, 38);

    gtk_box_pack_start(GTK_BOX(box), add_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), view_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), rem_btn, FALSE, FALSE, 4);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_driver_gui), dialog);
    g_signal_connect(view_btn, "clicked", G_CALLBACK(view_drivers_gui), dialog);
    g_signal_connect(rem_btn, "clicked", G_CALLBACK(remove_driver_gui), dialog);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


/* ================= 4. SCHEDULE MANAGEMENT ================= */

static void add_schedule_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Add Schedule",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Save Schedule",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 420, 280);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    GtkWidget *id_label = gtk_label_new("Schedule ID:");
    GtkWidget *bus_label = gtk_label_new("Bus ID:");
    GtkWidget *route_label = gtk_label_new("Route ID:");
    GtkWidget *start_label = gtk_label_new("Start Time (e.g. 08:00 AM):");
    GtkWidget *end_label = gtk_label_new("End Time (e.g. 05:00 PM):");
    GtkWidget *day_label = gtk_label_new("Day Type (Full Day / Half Day):");

    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *bus_entry = gtk_entry_new();
    GtkWidget *route_entry = gtk_entry_new();
    GtkWidget *start_entry = gtk_entry_new();
    GtkWidget *end_entry = gtk_entry_new();
    GtkWidget *day_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(start_entry), "08:00 AM");
    gtk_entry_set_text(GTK_ENTRY(end_entry), "05:00 PM");
    gtk_entry_set_text(GTK_ENTRY(day_entry), "Full Day");

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bus_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), route_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), route_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), start_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), start_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), end_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), end_entry, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), day_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), day_entry, 1, 5, 1, 1);

    gtk_container_add(GTK_CONTAINER(content), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int schedId = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        int busId = atoi(gtk_entry_get_text(GTK_ENTRY(bus_entry)));
        int routeId = atoi(gtk_entry_get_text(GTK_ENTRY(route_entry)));
        const char *startTime = gtk_entry_get_text(GTK_ENTRY(start_entry));
        const char *endTime = gtk_entry_get_text(GTK_ENTRY(end_entry));
        const char *dayType = gtk_entry_get_text(GTK_ENTRY(day_entry));

        if (schedId <= 0 || busId <= 0 || routeId <= 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid Input", "Please provide valid IDs for schedule, bus, and route.");
        }
        else
        {
            int res = addSchedule(schedId, busId, routeId, startTime, endTime, dayType);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Schedule added and saved successfully!");
            }
            else if (res == -2)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_WARNING, "Duplicate ID", "A schedule with this ID already exists.");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Error", "Could not add schedule. Limit reached or file error.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void view_schedules_gui(GtkWidget *widget, gpointer data)
{
    loadSchedules();

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "View Schedules",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 350);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(text_view), 10);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    char text[4096] = "";
    if (scheduleCount == 0)
    {
        snprintf(text, sizeof(text), "No schedule records found.\n");
    }
    else
    {
        char header[256];
        snprintf(header, sizeof(header), "%-10s | %-8s | %-10s | %-12s | %-12s | %-12s\n",
                 "SCHED ID", "BUS ID", "ROUTE ID", "START", "END", "DAY TYPE");
        strncat(text, header, sizeof(text) - strlen(text) - 1);
        strncat(text, "--------------------------------------------------------------------------------\n", sizeof(text) - strlen(text) - 1);

        for (int i = 0; i < scheduleCount; i++)
        {
            char line[256];
            snprintf(line, sizeof(line), "%-10d | %-8d | %-10d | %-12s | %-12s | %-12s\n",
                     schedules[i].scheduleId, schedules[i].busId, schedules[i].routeId,
                     schedules[i].startTime, schedules[i].endTime, schedules[i].dayType);
            strncat(text, line, sizeof(text) - strlen(text) - 1);
        }
    }

    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    gtk_container_add(GTK_CONTAINER(content), scrolled);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void remove_schedule_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remove Schedule",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Remove",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 15);

    GtkWidget *label = gtk_label_new("Enter Schedule ID to remove:");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(content), box);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        int schedId = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
        if (schedId <= 0)
        {
            show_alert(GTK_WINDOW(dialog), GTK_MESSAGE_ERROR, "Invalid ID", "Please enter a valid positive Schedule ID.");
        }
        else
        {
            int res = removeSchedule(schedId);
            if (res == 0)
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_INFO, "Success", "Schedule removed successfully!");
            }
            else
            {
                show_alert(GTK_WINDOW(data), GTK_MESSAGE_ERROR, "Not Found", "Schedule ID not found.");
            }
        }
    }

    gtk_widget_destroy(dialog);
}

static void schedule_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Schedule Management",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_CLOSE,
        NULL
    );

    gtk_window_set_default_size(GTK_WINDOW(dialog), 350, 220);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 20);

    GtkWidget *add_btn = gtk_button_new_with_label("➕  Add Schedule");
    GtkWidget *view_btn = gtk_button_new_with_label("⏰  View Schedules");
    GtkWidget *rem_btn = gtk_button_new_with_label("🗑️  Remove Schedule");

    gtk_widget_set_size_request(add_btn, -1, 38);
    gtk_widget_set_size_request(view_btn, -1, 38);
    gtk_widget_set_size_request(rem_btn, -1, 38);

    gtk_box_pack_start(GTK_BOX(box), add_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), view_btn, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), rem_btn, FALSE, FALSE, 4);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_schedule_gui), dialog);
    g_signal_connect(view_btn, "clicked", G_CALLBACK(view_schedules_gui), dialog);
    g_signal_connect(rem_btn, "clicked", G_CALLBACK(remove_schedule_gui), dialog);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


/* ================= 5. MAIN MEMBER 2 GUI WINDOW ================= */

void member2_gui(void)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *header;
    GtkWidget *sub_header;

    GtkWidget *bus_button;
    GtkWidget *route_button;
    GtkWidget *driver_button;
    GtkWidget *schedule_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(
        GTK_WINDOW(window),
        "BusMate - Management Dashboard"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        480,
        420
    );

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_container_set_border_width(
        GTK_CONTAINER(window),
        25
    );

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        14
    );

    header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header), "<span size='x-large' weight='bold' color='#1a365d'>Transport Management Panel</span>");

    sub_header = gtk_label_new("Manage College Buses, Routes, Drivers & Schedules");

    gtk_box_pack_start(GTK_BOX(box), header, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), sub_header, FALSE, FALSE, 4);

    bus_button = gtk_button_new_with_label("🚌  Bus Management");
    route_button = gtk_button_new_with_label("🗺️  Route Management");
    driver_button = gtk_button_new_with_label("👤  Driver Management");
    schedule_button = gtk_button_new_with_label("⏰  Schedule Management");

    /* Set comfortable button heights so they are easily clickable and prominently visible */
    gtk_widget_set_size_request(bus_button, -1, 46);
    gtk_widget_set_size_request(route_button, -1, 46);
    gtk_widget_set_size_request(driver_button, -1, 46);
    gtk_widget_set_size_request(schedule_button, -1, 46);

    gtk_box_pack_start(GTK_BOX(box), bus_button, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), route_button, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), driver_button, FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(box), schedule_button, FALSE, FALSE, 4);

    g_signal_connect(
        bus_button,
        "clicked",
        G_CALLBACK(bus_clicked),
        window
    );

    g_signal_connect(
        route_button,
        "clicked",
        G_CALLBACK(route_clicked),
        window
    );

    g_signal_connect(
        driver_button,
        "clicked",
        G_CALLBACK(driver_clicked),
        window
    );

    g_signal_connect(
        schedule_button,
        "clicked",
        G_CALLBACK(schedule_clicked),
        window
    );

    gtk_container_add(
        GTK_CONTAINER(window),
        box
    );

    gtk_widget_show_all(window);
}


/* ================= STANDALONE RUNNER ================= */

#ifdef STANDALONE_MEMBER2
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    member2_gui();

    gtk_main();

    return 0;
}
#endif