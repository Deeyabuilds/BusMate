#include "SeatManager.h"
#include <stdio.h>
#include <string.h>

#define DATA_FILE "data/seats.dat"

void init_seats_file()
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
    {
        fp = fopen(DATA_FILE, "wb");
        if (fp)
        {
            Seat empty_seat;
            for (int i = 1; i <= 40; i++)
            {
                empty_seat.seat_id = i;
                empty_seat.is_booked = 0;
                strcpy(empty_seat.passenger_name, "None");
                fwrite(&empty_seat, sizeof(Seat), 1, fp);
            }
            fclose(fp);
        }
    }
    else
    {
        fclose(fp);
    }
}

int book_seat_binary(int seat_id, const char *passenger_name)
{
    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp)
        return 0;

    Seat seat;
    fseek(fp, (seat_id - 1) * sizeof(Seat), SEEK_SET);

    seat.seat_id = seat_id;
    seat.is_booked = 1;
    strncpy(seat.passenger_name, passenger_name, 49);
    seat.passenger_name[49] = '\0';

    fwrite(&seat, sizeof(Seat), 1, fp);
    fclose(fp);
    return 1;
}

int is_seat_booked(int seat_id)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return 0;

    Seat seat;
    fseek(fp, (seat_id - 1) * sizeof(Seat), SEEK_SET);
    if (fread(&seat, sizeof(Seat), 1, fp) == 1)
    {
        fclose(fp);
        return seat.is_booked;
    }

    fclose(fp);
    return 0;
}