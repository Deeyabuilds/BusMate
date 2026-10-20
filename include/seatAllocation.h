#ifndef SEAT_ALLOCATION_H
#define SEAT_ALLOCATION_H

/* Seat Allocation Menu */
void seatAllocationMenu(void);

/* Seat Availability */
void viewAvailableSeats(void);
void viewSeatDetails(void);

/* Seat Booking */
void allocateSeat(void);
void cancelSeatAllocation(void);

/* Seat Preferences */
void setSeatPreference(void);

/* Waiting List */
void addToWaitingList(void);
void viewWaitingList(void);

/* Occupancy */
void viewBusOccupancy(void);

#endif