#include <gtk/gtk.h>
#include "../include/WaitingList.h"

static WaitingQueue *queue = NULL;
static GtkListStore *list_store = NULL;
static GtkWidget *entry_id, *entry_name, *entry_route;

// Table refresh function
void refresh_list_view()
{
    gtk_list_store_clear(list_store);
    StudentNode *curr = queue->front;
    int pos = 1;
    GtkTreeIter iter;

    while (curr != NULL)
    {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                           0, pos++,
                           1, curr->student_id,
                           2, curr->name,
                           3, curr->route_id,
                           -1);
        curr = curr->next;
    }
}

// Enqueue button callback
void on_add_clicked(GtkWidget *widget, gpointer data)
{
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *route = gtk_entry_get_text(GTK_ENTRY(entry_route));

    if (strlen(id) > 0 && strlen(name) > 0 && strlen(route) > 0)
    {
        enqueue_student(queue, id, name, route);
        save_waiting_list(queue, "waiting_list.dat");
        refresh_list_view();

        // Clear entry inputs
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");
        gtk_entry_set_text(GTK_ENTRY(entry_name), "");
        gtk_entry_set_text(GTK_ENTRY(entry_route), "");
    }
}

// Dequeue button callback (Seat Allocated)
void on_allocate_clicked(GtkWidget *widget, gpointer data)
{
    StudentNode *served = dequeue_student(queue);
    if (served != NULL)
    {
        save_waiting_list(queue, "waiting_list.dat");
        refresh_list_view();
        free(served);
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    queue = create_queue();
    load_waiting_list(queue, "waiting_list.dat");

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BusMate - Waiting List Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    // Inputs form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);

    entry_id = gtk_entry_new();
    entry_name = gtk_entry_new();
    entry_route = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Student ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Route ID:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_route, 1, 2, 1, 1);

    GtkWidget *btn_add = gtk_button_new_with_label("Add to Waiting List");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_add, 1, 3, 1, 1);

    gtk_box_pack_start(GTK_BOX(main_vbox), grid, FALSE, FALSE, 0);

    // List view tree setup
    list_store = gtk_list_store_new(4, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), gtk_tree_view_column_new_with_attributes("Pos", renderer, "text", 0, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 1, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 2, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), gtk_tree_view_column_new_with_attributes("Route", renderer, "text", 3, NULL));
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);
    gtk_box_pack_start(GTK_BOX(main_vbox), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *btn_allocate = gtk_button_new_with_label("Allocate Next Available Seat (Dequeue)");
    g_signal_connect(btn_allocate, "clicked", G_CALLBACK(on_allocate_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), btn_allocate, FALSE, FALSE, 0);

    refresh_list_view();

    gtk_widget_show_all(window);
    gtk_main();

    free_queue(queue);
    return 0;
}