#ifndef SEAT_ALLOCATION_H
#define SEAT_ALLOCATION_H

#include <stdio.h>
#include <stddef.h>
#include "structures.h"

enum
{
    SEAT_ALLOCATION_ERROR = 0,
    SEAT_ALLOCATION_BOOKED = 1,
    SEAT_ALLOCATION_WAITLISTED = 2
};

int initializeBusSeats(int busId);
int setSeatPreference(int busId, int seatNumber, const char *preference);
int getBusSeats(int busId, Seat **seats, size_t *count);
int displaySeatAvailability(int busId, FILE *output);
int allocateSeat(int busId, int studentId, const char *preference, Seat *allocatedSeat);
int allocateSpecificSeat(int busId, int studentId, int seatNumber,
                         const char *preference, Seat *allocatedSeat);
int cancelSeat(int busId, int studentId, Seat *freedSeat, int *promotedStudentId);
int getBusOccupancy(int busId, int *bookedSeats, int *capacity);
int displayBusOccupancy(int busId, FILE *output);
int displayWaitingList(int busId, FILE *output);
int getWaitingList(int busId, int **studentIds, size_t *count);

#endif