#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

#include "structures.h"
#include "file_handler.h"

// Prototypes from original binary implementation
void init_seats_file();
int book_seat_binary(int seat_id, const char *passenger_name);
int is_seat_booked(int seat_id);

// Preference Allocation Prototypes
#define TOTAL_ROWS 10
#define SEATS_PER_ROW 4
#define TOTAL_SEATS (TOTAL_ROWS * SEATS_PER_ROW)

void init_seat_system(void);
int allocate_seat(int student_id, const char *name, SeatPreference pref);
bool cancel_seat(int student_id);
void display_bus_occupancy(void);

#endif