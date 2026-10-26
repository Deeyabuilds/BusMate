#ifndef STUDENT_H
#define STUDENT_H

#include <gtk/gtk.h>
#include "structures.h"

void show_student_login(GtkWindow *parent);
void show_student_dashboard(GtkWindow *parent);

int register_student(Student student);
int login_student(const char *student_id, const char *password);

#endif