#ifndef ADMIN_H
#define ADMIN_H

#include <gtk/gtk.h>
#include "structures.h"

void show_admin_login(GtkWindow *parent);
void show_admin_dashboard(GtkWindow *parent);

int admin_login(const char *username, const char *password);
int register_admin(Admin admin);

#endif