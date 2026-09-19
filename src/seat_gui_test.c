#include <gtk/gtk.h>
#include "../include/SeatManager.h"
#include "../include/structures.h"

static GtkWidget *entry_student_id;
static GtkWidget *entry_name;
static GtkWidget *combo_pref;
static GtkWidget *label_result;

static void on_allocate_clicked(GtkWidget *widget, gpointer data)
{
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_student_id));
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *pref = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_pref));

    int student_id = atoi(id_text);

    if (student_id <= 0 || strlen(name) == 0)
    {
        gtk_label_set_text(GTK_LABEL(label_result), "Status: Invalid Input");
        return;
    }

    // Updated function call matching new SeatManager logic
    int seat_num = allocate_seat(student_id, name, pref ? pref : "Any");

    char buffer[100];
    if (seat_num > 0)
    {
        snprintf(buffer, sizeof(buffer), "Status: Allocated Seat #%d", seat_num);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "Status: Bus Full! Added to Waiting List");
    }
    gtk_label_set_text(GTK_LABEL(label_result), buffer);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    init_seat_system();
    init_seats_file();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Seat Allocation Test");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 250);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 15);

    entry_student_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_student_id), "Student ID (e.g. 101)");
    gtk_box_pack_start(GTK_BOX(vbox), entry_student_id, FALSE, FALSE, 0);

    entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name), "Student Name");
    gtk_box_pack_start(GTK_BOX(vbox), entry_name, FALSE, FALSE, 0);

    combo_pref = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_pref), "Window");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_pref), "Front");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_pref), "Any");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_pref), 2);
    gtk_box_pack_start(GTK_BOX(vbox), combo_pref, FALSE, FALSE, 0);

    GtkWidget *btn_allocate = gtk_button_new_with_label("Allocate Seat");
    g_signal_connect(btn_allocate, "clicked", G_CALLBACK(on_allocate_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn_allocate, FALSE, FALSE, 0);

    label_result = gtk_label_new("Status: Ready");
    gtk_box_pack_start(GTK_BOX(vbox), label_result, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}