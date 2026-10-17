#include <gtk/gtk.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>
#include "gui.h"
#include "file_handler.h"
#include "seat_allocation.h"

typedef struct
{
    GtkWidget *window;
    GtkWidget *bus_combo;
    GtkWidget *student_id_entry;
    GtkWidget *preference_combo;
    GtkWidget *seat_grid;
    GtkWidget *summary_label;
    GtkWidget *status_label;
    int selected_seat;
    int loading_buses;
} SeatWindow;

typedef struct
{
    GtkWidget *window;
    GtkWidget *bus_id_entry;
    GtkWidget *passenger_view;
    GtkWidget *status_label;
} DriverWindow;

static int read_positive_id(GtkWidget *entry, int *id)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);
    if (text == end || errno == ERANGE || value <= 0 || value > INT_MAX)
        return 0;
    while (*end == ' ' || *end == '\t')
        end++;
    if (*end != '\0')
        return 0;
    *id = (int)value;
    return 1;
}

static int authenticate_student(int studentId);

static const char *get_preference(SeatWindow *context)
{
    return gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(context->preference_combo));
}

static int read_selected_bus_id(SeatWindow *context, int *busId)
{
    const char *activeId = gtk_combo_box_get_active_id(GTK_COMBO_BOX(context->bus_combo));
    char *end;
    long value;

    if (activeId == NULL)
        return 0;
    errno = 0;
    value = strtol(activeId, &end, 10);
    if (activeId == end || *end != '\0' || errno == ERANGE || value <= 0 || value > INT_MAX)
        return 0;
    *busId = (int)value;
    return 1;
}

static void clear_seat_grid(SeatWindow *context)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(context->seat_grid));
    GList *item;

    for (item = children; item != NULL; item = item->next)
        gtk_widget_destroy(GTK_WIDGET(item->data));
    g_list_free(children);
}

static void on_seat_clicked(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = user_data;
    int seatNumber = GPOINTER_TO_INT(
        g_object_get_data(G_OBJECT(button), "seat-number"));
    char message[80];

    context->selected_seat = seatNumber;
    g_snprintf(message, sizeof(message), "Seat %d selected.", seatNumber);
    gtk_label_set_text(GTK_LABEL(context->status_label), message);
}

static void refresh_seat_grid(SeatWindow *context)
{
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;
    int busId;
    int bookedSeats = 0;
    int capacity = 0;
    char summary[120];

    clear_seat_grid(context);
    context->selected_seat = 0;
    if (!read_selected_bus_id(context, &busId))
    {
        gtk_label_set_text(GTK_LABEL(context->status_label), "Select a bus from the list.");
        gtk_label_set_text(GTK_LABEL(context->summary_label),
                           "Total seats: -- | Booked: -- | Available: --");
        gtk_widget_show_all(context->seat_grid);
        return;
    }
    if (!getBusSeats(busId, &seats, &count) ||
        !getBusOccupancy(busId, &bookedSeats, &capacity))
    {
        free(seats);
        gtk_label_set_text(GTK_LABEL(context->summary_label), "Total seats: -- | Booked: -- | Available: --");
        gtk_label_set_text(GTK_LABEL(context->status_label), "Bus records could not be loaded.");
        gtk_widget_show_all(context->seat_grid);
        return;
    }
    g_snprintf(summary, sizeof(summary), "Total seats: %d | Booked: %d | Available: %d",
               capacity, bookedSeats, capacity - bookedSeats);
    gtk_label_set_text(GTK_LABEL(context->summary_label), summary);
    if (count == 0)
        gtk_label_set_text(GTK_LABEL(context->status_label), "This bus has no seats.");
    else
        gtk_label_set_text(GTK_LABEL(context->status_label), "Choose a seat, then book or cancel.");

    for (index = 0; index < count; index++)
    {
        GtkWidget *button;
        char label[80];
        int column = (int)(index % 5);
        int row = (int)(index / 5);
        int available = g_strcmp0(seats[index].status, "Available") == 0;

        g_snprintf(label, sizeof(label), "Seat %d\n%s | %s", seats[index].seatNumber,
                   seats[index].status, seats[index].preference);
        button = gtk_button_new_with_label(label);
        gtk_widget_set_size_request(button, 125, 64);
        gtk_widget_set_sensitive(button, available);
        g_object_set_data(G_OBJECT(button), "seat-number",
                          GINT_TO_POINTER(seats[index].seatNumber));
        g_signal_connect(button, "clicked", G_CALLBACK(on_seat_clicked), context);
        gtk_grid_attach(GTK_GRID(context->seat_grid), button, column, row, 1, 1);
    }
    free(seats);
    gtk_widget_show_all(context->seat_grid);
}

static void on_load_bus_clicked(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = user_data;
    Bus *buses = NULL;
    void *records = NULL;
    size_t count = 0;
    size_t index;

    (void)button;
    context->loading_buses = 1;
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(context->bus_combo));
    if (!readRecords(BUSES_FILE, sizeof(Bus), &records, &count))
    {
        context->loading_buses = 0;
        clear_seat_grid(context);
        gtk_label_set_text(GTK_LABEL(context->summary_label),
                           "Total seats: -- | Booked: -- | Available: --");
        gtk_label_set_text(GTK_LABEL(context->status_label), "Could not read buses.dat.");
        gtk_widget_show_all(context->seat_grid);
        return;
    }

    buses = records;
    for (index = 0; index < count; index++)
    {
        char *id = g_strdup_printf("%d", buses[index].busId);
        char *label = g_strdup_printf("Bus %d - %s (%d seats)", buses[index].busId,
                                      buses[index].busNumber, buses[index].capacity);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(context->bus_combo), id, label);
        g_free(id);
        g_free(label);
    }
    free(records);
    context->loading_buses = 0;
    gtk_combo_box_set_active(GTK_COMBO_BOX(context->bus_combo), count == 0 ? -1 : 0);
    if (count == 0)
        refresh_seat_grid(context);
}

static void on_preference_changed(GtkComboBox *combo, gpointer user_data)
{
    SeatWindow *context = user_data;
    gchar *preference;
    char message[100];

    if (context->loading_buses || GTK_WIDGET(combo) == context->bus_combo)
        return;
    preference = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (preference != NULL)
    {
        g_snprintf(message, sizeof(message),
                   "Preference: %s. Select an available seat or allocate automatically.",
                   preference);
        gtk_label_set_text(GTK_LABEL(context->status_label), message);
        g_free(preference);
    }
}

static void on_bus_selected(GtkComboBox *combo, gpointer user_data)
{
    SeatWindow *context = user_data;

    (void)combo;
    if (!context->loading_buses)
        refresh_seat_grid(context);
}

static void on_allocate_clicked(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = user_data;
    Seat allocated;
    const char *preference;
    int busId;
    int studentId;
    int result;
    char message[100];

    (void)button;
    if (!read_selected_bus_id(context, &busId) ||
        !read_positive_id(context->student_id_entry, &studentId) ||
        !authenticate_student(studentId))
    {
        gtk_label_set_text(GTK_LABEL(context->status_label),
                           "Select a bus and enter a registered student ID.");
        return;
    }
    preference = get_preference(context);
    if (preference == NULL)
    {
        gtk_label_set_text(GTK_LABEL(context->status_label), "Choose a seat preference.");
        return;
    }
    if (context->selected_seat != 0)
        result = allocateSpecificSeat(busId, studentId, context->selected_seat,
                                      preference, &allocated);
    else
        result = allocateSeat(busId, studentId, preference, &allocated);
    g_free((gpointer)preference);

    if (result == SEAT_ALLOCATION_BOOKED)
        g_snprintf(message, sizeof(message), "Seat %d booked for student %d.",
                   allocated.seatNumber, studentId);
    else if (result == SEAT_ALLOCATION_WAITLISTED)
        g_snprintf(message, sizeof(message), "Bus full. Student %d joined the waiting list.",
                   studentId);
    else
        g_snprintf(message, sizeof(message), "Unable to allocate a matching seat.");
    gtk_label_set_text(GTK_LABEL(context->status_label), message);
    refresh_seat_grid(context);
    gtk_label_set_text(GTK_LABEL(context->status_label), message);
}

static void on_cancel_clicked(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = user_data;
    Seat freed;
    int busId;
    int studentId;
    int promotedStudentId = 0;
    SeatCancellationResult cancellationResult;
    char message[120];

    (void)button;
    if (!read_selected_bus_id(context, &busId) ||
        !read_positive_id(context->student_id_entry, &studentId) ||
        !authenticate_student(studentId))
    {
        gtk_label_set_text(GTK_LABEL(context->status_label),
                           "Select a bus and enter a registered student ID.");
        return;
    }
    cancellationResult = cancelSeat(busId, studentId, &freed, &promotedStudentId);
    if (cancellationResult == SEAT_CANCELLATION_ERROR ||
        cancellationResult == SEAT_CANCELLATION_NOT_FOUND)
    {
        gtk_label_set_text(GTK_LABEL(context->status_label),
                           cancellationResult == SEAT_CANCELLATION_NOT_FOUND ?
                           "No booked seat was found for this student." :
                           "Could not update the seat files; cancellation failed.");
        return;
    }
    if (cancellationResult == SEAT_CANCELLATION_PROMOTION_FAILED)
        g_snprintf(message, sizeof(message),
                   "Seat %d cancelled, but waiting-list promotion failed. Retry after checking storage.",
                   freed.seatNumber);
    else if (cancellationResult == SEAT_CANCELLATION_PROMOTED && promotedStudentId != 0)
        g_snprintf(message, sizeof(message), "Seat %d freed; student %d was promoted.",
                   freed.seatNumber, promotedStudentId);
    else
        g_snprintf(message, sizeof(message), "Seat %d cancelled.", freed.seatNumber);
    refresh_seat_grid(context);
    gtk_label_set_text(GTK_LABEL(context->status_label), message);
}

static void on_waiting_list_clicked(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = user_data;
    int *studentIds = NULL;
    size_t count = 0;
    size_t index;
    int busId;
    GString *text;
    GtkWidget *dialog;

    (void)button;
    if (!read_selected_bus_id(context, &busId) ||
        !getWaitingList(busId, &studentIds, &count))
    {
        gtk_label_set_text(GTK_LABEL(context->status_label),
                           "Select a bus and check its waiting-list records.");
        return;
    }
    text = g_string_new(NULL);
    if (count == 0)
        g_string_append(text, "No students are waiting for this bus.");
    else
        for (index = 0; index < count; index++)
            g_string_append_printf(text, "%zu. Student %d\n", index + 1, studentIds[index]);
    dialog = gtk_message_dialog_new(GTK_WINDOW(context->window), GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
                                    "Waiting list for bus %d", busId);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", text->str);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_string_free(text, TRUE);
    free(studentIds);
}

static void on_seat_window_destroy(GtkWidget *widget, gpointer user_data)
{
    (void)widget;
    g_free(user_data);
}

static void show_seat_allocation(GtkButton *button, gpointer user_data)
{
    SeatWindow *context = g_new0(SeatWindow, 1);
    GtkWidget *box;
    GtkWidget *form_grid;
    GtkWidget *scroll_window;
    GtkWidget *load_button;
    GtkWidget *allocate_button;
    GtkWidget *cancel_button;
    GtkWidget *waiting_button;
    GtkWidget *label;

    (void)button;
    context->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(context->window), "BusMate - Seat Allocation");
    gtk_window_set_default_size(GTK_WINDOW(context->window), 760, 600);
    gtk_window_set_position(GTK_WINDOW(context->window), GTK_WIN_POS_CENTER);
    if (GTK_IS_WINDOW(user_data))
        gtk_window_set_transient_for(GTK_WINDOW(context->window), GTK_WINDOW(user_data));
    g_signal_connect(context->window, "destroy",
                     G_CALLBACK(on_seat_window_destroy), context);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 16);
    gtk_container_add(GTK_CONTAINER(context->window), box);

    form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 8);
    gtk_box_pack_start(GTK_BOX(box), form_grid, FALSE, FALSE, 0);

    label = gtk_label_new("Bus");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), label, 0, 0, 1, 1);
    context->bus_combo = gtk_combo_box_text_new();
    gtk_widget_set_hexpand(context->bus_combo, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), context->bus_combo, 1, 0, 2, 1);

    label = gtk_label_new("Student ID");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), label, 0, 1, 1, 1);
    context->student_id_entry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(context->student_id_entry), GTK_INPUT_PURPOSE_DIGITS);
    gtk_entry_set_placeholder_text(GTK_ENTRY(context->student_id_entry), "Registered student ID");
    gtk_grid_attach(GTK_GRID(form_grid), context->student_id_entry, 1, 1, 1, 1);

    label = gtk_label_new("Preference");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), label, 2, 1, 1, 1);
    context->preference_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(context->preference_combo), "Window");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(context->preference_combo), "Front");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(context->preference_combo), "Any");
    gtk_combo_box_set_active(GTK_COMBO_BOX(context->preference_combo), 2);
    gtk_grid_attach(GTK_GRID(form_grid), context->preference_combo, 3, 1, 1, 1);

    load_button = gtk_button_new_with_label("Reload Buses / Seats");
    gtk_grid_attach(GTK_GRID(form_grid), load_button, 3, 0, 1, 1);
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_bus_clicked), context);
    g_signal_connect(context->bus_combo, "changed", G_CALLBACK(on_bus_selected), context);
    g_signal_connect(context->preference_combo, "changed",
                     G_CALLBACK(on_preference_changed), context);

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(box), scroll_window, TRUE, TRUE, 0);
    context->seat_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(context->seat_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(context->seat_grid), 8);
    gtk_container_add(GTK_CONTAINER(scroll_window), context->seat_grid);

    context->summary_label = gtk_label_new("Total seats: -- | Booked: -- | Available: --");
    gtk_widget_set_halign(context->summary_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), context->summary_label, FALSE, FALSE, 0);

    context->status_label = gtk_label_new("Loading buses...");
    gtk_widget_set_halign(context->status_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), context->status_label, FALSE, FALSE, 0);

    allocate_button = gtk_button_new_with_label("Allocate Seat");
    cancel_button = gtk_button_new_with_label("Cancel My Seat");
    waiting_button = gtk_button_new_with_label("View Waiting List");
    gtk_box_pack_start(GTK_BOX(box), allocate_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), cancel_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), waiting_button, FALSE, FALSE, 0);
    g_signal_connect(allocate_button, "clicked", G_CALLBACK(on_allocate_clicked), context);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_cancel_clicked), context);
    g_signal_connect(waiting_button, "clicked", G_CALLBACK(on_waiting_list_clicked), context);

    gtk_widget_show_all(context->window);
    on_load_bus_clicked(GTK_BUTTON(load_button), context);
}

static int authenticate_student(int studentId)
{
    Student *students = NULL;
    void *records = NULL;
    size_t count = 0;
    size_t index;
    int authenticated = 0;

    if (!readRecords(STUDENTS_FILE, sizeof(Student), &records, &count))
        return 0;
    students = records;
    for (index = 0; index < count; index++)
        if (students[index].studentId == studentId)
        {
            authenticated = 1;
            break;
        }
    free(records);
    return authenticated;
}

static void show_admin_bus_setup(GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog;
    GtkWidget *content;
    GtkWidget *grid;
    GtkWidget *bus_id_entry;
    GtkWidget *bus_number_entry;
    GtkWidget *capacity_entry;
    GtkWidget *label;
    int response;

    (void)button;
    dialog = gtk_dialog_new_with_buttons("Add Bus", GTK_WINDOW(user_data), GTK_DIALOG_MODAL,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         "Add Bus", GTK_RESPONSE_ACCEPT, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 420, 220);
    content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 16);
    gtk_box_pack_start(GTK_BOX(content), grid, TRUE, TRUE, 0);

    label = gtk_label_new("Bus ID");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    bus_id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(bus_id_entry), "Positive integer");
    gtk_grid_attach(GTK_GRID(grid), bus_id_entry, 1, 0, 1, 1);
    label = gtk_label_new("Bus number");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    bus_number_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), bus_number_entry, 1, 1, 1, 1);
    label = gtk_label_new("Capacity");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    capacity_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(capacity_entry), "For example: 40");
    gtk_grid_attach(GTK_GRID(grid), capacity_entry, 1, 2, 1, 1);
    gtk_widget_show_all(dialog);
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        Bus bus;
        Bus *existingBuses = NULL;
        void *records = NULL;
        size_t count = 0;
        size_t index;
        int busId;
        int capacity;
        const char *busNumber = gtk_entry_get_text(GTK_ENTRY(bus_number_entry));
        const char *message;
        GtkWidget *resultDialog;

        if (!read_positive_id(bus_id_entry, &busId) ||
            !read_positive_id(capacity_entry, &capacity) || busNumber[0] == '\0' ||
            strlen(busNumber) >= sizeof(bus.busNumber))
            message = "Enter a positive bus ID and capacity, and a bus number under 20 characters.";
        else if (!readRecords(BUSES_FILE, sizeof(Bus), &records, &count))
            message = "Could not read buses.dat.";
        else
        {
            existingBuses = records;
            for (index = 0; index < count; index++)
                if (existingBuses[index].busId == busId)
                    break;
            free(records);
            if (index != count)
                message = "That bus ID already exists.";
            else
            {
                memset(&bus, 0, sizeof(bus));
                bus.busId = busId;
                bus.capacity = capacity;
                bus.availableSeats = capacity;
                g_strlcpy(bus.busNumber, busNumber, sizeof(bus.busNumber));
                if (!appendRecord(BUSES_FILE, sizeof(bus), &bus))
                    message = "Could not save the bus record.";
                else if (!initializeBusSeats(busId))
                {
                    int deleted = 0;
                    deleteRecordById(SEATS_FILE, sizeof(Seat), offsetof(Seat, busId),
                                     busId, &deleted);
                    deleteRecordById(BUSES_FILE, sizeof(Bus), offsetof(Bus, busId),
                                     busId, &deleted);
                    message = "Could not initialize the seats; the bus record was rolled back.";
                }
                else
                    message = "Bus added and all seats generated. Reload buses in the Student screen.";
            }
        }
        resultDialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL,
                                              GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
                                              "%s", message);
        gtk_dialog_run(GTK_DIALOG(resultDialog));
        gtk_widget_destroy(resultDialog);
    }
    gtk_widget_destroy(dialog);
}

static void on_driver_refresh(GtkButton *button, gpointer user_data)
{
    DriverWindow *context = user_data;
    Seat *seats = NULL;
    Student *students = NULL;
    void *studentRecords = NULL;
    size_t seatCount = 0;
    size_t studentCount = 0;
    size_t index;
    int busId;
    int booked = 0;
    int capacity = 0;
    GString *passengers;
    GtkTextBuffer *buffer;

    (void)button;
    if (!read_positive_id(context->bus_id_entry, &busId) ||
        !getBusSeats(busId, &seats, &seatCount) ||
        !getBusOccupancy(busId, &booked, &capacity) ||
        !readRecords(STUDENTS_FILE, sizeof(Student), &studentRecords, &studentCount))
    {
        free(seats);
        free(studentRecords);
        gtk_label_set_text(GTK_LABEL(context->status_label),
                           "Enter a valid bus ID and check that its records are available.");
        return;
    }

    students = studentRecords;
    passengers = g_string_new(NULL);
    for (index = 0; index < seatCount; index++)
    {
        size_t studentIndex;
        if (g_strcmp0(seats[index].status, "Booked") != 0)
            continue;
        g_string_append_printf(passengers, "Seat %d: student %d",
                               seats[index].seatNumber, seats[index].studentId);
        for (studentIndex = 0; studentIndex < studentCount; studentIndex++)
            if (students[studentIndex].studentId == seats[index].studentId)
            {
                g_string_append_printf(passengers, " (%s)", students[studentIndex].name);
                break;
            }
        g_string_append_c(passengers, '\n');
    }
    if (booked == 0)
        g_string_append(passengers, "No booked seats.\n");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(context->passenger_view));
    gtk_text_buffer_set_text(buffer, passengers->str, -1);
    {
        char summary[100];
        g_snprintf(summary, sizeof(summary),
                   "Total seats: %d | Booked: %d | Available: %d",
                   capacity, booked, capacity - booked);
        gtk_label_set_text(GTK_LABEL(context->status_label), summary);
    }
    g_string_free(passengers, TRUE);
    free(studentRecords);
    free(seats);
}

static void on_driver_window_destroy(GtkWidget *widget, gpointer user_data)
{
    (void)widget;
    g_free(user_data);
}

static void show_driver_occupancy(GtkButton *button, gpointer user_data)
{
    DriverWindow *context = g_new0(DriverWindow, 1);
    GtkWidget *box;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *refresh_button;
    GtkWidget *scroll;

    (void)button;
    context->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(context->window), "BusMate - Bus Occupancy");
    gtk_window_set_default_size(GTK_WINDOW(context->window), 620, 480);
    if (GTK_IS_WINDOW(user_data))
        gtk_window_set_transient_for(GTK_WINDOW(context->window), GTK_WINDOW(user_data));
    g_signal_connect(context->window, "destroy",
                     G_CALLBACK(on_driver_window_destroy), context);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 16);
    gtk_container_add(GTK_CONTAINER(context->window), box);
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 0);
    label = gtk_label_new("Bus ID");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    context->bus_id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(context->bus_id_entry), "Enter bus ID");
    gtk_grid_attach(GTK_GRID(grid), context->bus_id_entry, 1, 0, 1, 1);
    refresh_button = gtk_button_new_with_label("Show Occupancy");
    gtk_grid_attach(GTK_GRID(grid), refresh_button, 2, 0, 1, 1);
    g_signal_connect(refresh_button, "clicked", G_CALLBACK(on_driver_refresh), context);

    context->status_label = gtk_label_new("Enter a bus ID to view passengers.");
    gtk_widget_set_halign(context->status_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), context->status_label, FALSE, FALSE, 0);
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);
    context->passenger_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(context->passenger_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(context->passenger_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scroll), context->passenger_view);
    gtk_widget_show_all(context->window);
}

static void show_role_screen(GtkWidget *widget, gpointer data)
{
    GtkWidget *role_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    GtkWidget *title = gtk_label_new("SELECT ROLE");
    GtkWidget *student_button = gtk_button_new_with_label("Student");
    GtkWidget *driver_button = gtk_button_new_with_label("Driver");
    GtkWidget *admin_button = gtk_button_new_with_label("Admin");

    (void)widget;
    gtk_window_set_title(GTK_WINDOW(role_window), "BusMate - Select Role");
    gtk_window_set_default_size(GTK_WINDOW(role_window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(role_window), GTK_WIN_POS_CENTER);
    if (GTK_IS_WINDOW(data))
        gtk_window_set_transient_for(GTK_WINDOW(role_window), GTK_WINDOW(data));
    gtk_container_set_border_width(GTK_CONTAINER(box), 40);

    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box), student_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), driver_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), admin_button, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(role_window), box);

    g_signal_connect(student_button, "clicked", G_CALLBACK(show_seat_allocation), role_window);
    g_signal_connect(driver_button, "clicked", G_CALLBACK(show_driver_occupancy), role_window);
    g_signal_connect(admin_button, "clicked", G_CALLBACK(show_admin_bus_setup), role_window);
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