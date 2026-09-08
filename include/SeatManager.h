#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

typedef struct
{
    int seat_id;
    int is_booked;
    char passenger_name[50];
} Seat;

void init_seats_file();
int book_seat_binary(int seat_id, const char *passenger_name);
int is_seat_booked(int seat_id);

#endif