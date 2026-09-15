#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

#include "structures.h"
#include "file_handler.h"
#include <stdbool.h>

#define TOTAL_ROWS 10
#define SEATS_PER_ROW 4
#define TOTAL_SEATS (TOTAL_ROWS * SEATS_PER_ROW)

// Binary File Functions
void init_seats_file(void);
int book_seat_binary(int seat_id, int bus_id, int student_id, const char *preference);
int is_seat_booked(int seat_id, int bus_id);

// System Allocation Functions
void init_seat_system(void);
int allocate_seat(int student_id, const char *name, const char *pref);
bool cancel_seat(int student_id);
void display_bus_occupancy(void);

#endif