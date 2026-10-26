#ifndef BOOKING_H
#define BOOKING_H

#include "structures.h"

int create_booking(Booking booking);
int view_bookings(void);
int cancel_booking(const char *student_id,
                   const char *schedule_id);

#endif