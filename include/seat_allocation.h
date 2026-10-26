#ifndef SEAT_ALLOCATION_H
#define SEAT_ALLOCATION_H

#include "structures.h"

int allocate_seat(Booking *booking);
int check_seat_availability(const char *schedule_id,
                            int seat_number);

#endif