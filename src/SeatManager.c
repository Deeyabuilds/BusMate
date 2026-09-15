Bestie :
#include "../include/SeatManager.h"
#include <stdio.h>
#include <string.h>

#define DATA_FILE "data/seats.dat"

    void init_seats_file(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
    {
        fp = fopen(DATA_FILE, "wb");
        if (fp)
        {
            Seat empty_seat;
            for (int i = 1; i <= TOTAL_SEATS; i++)
            {
                empty_seat.seatNumber = i;
                empty_seat.busId = 101; // Default Bus ID
                empty_seat.studentId = 0;
                strcpy(empty_seat.status, "Available");
                strcpy(empty_seat.preference, "Any");
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

int book_seat_binary(int seat_id, int bus_id, int student_id, const char *preference)
{
    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp)
        return 0;

    Seat seat;
    fseek(fp, (seat_id - 1) * sizeof(Seat), SEEK_SET);

    seat.seatNumber = seat_id;
    seat.busId = bus_id;
    seat.studentId = student_id;

    strncpy(seat.status, "Booked", sizeof(seat.status) - 1);
    seat.status[sizeof(seat.status) - 1] = '\0';

    if (preference)
    {
        strncpy(seat.preference, preference, sizeof(seat.preference) - 1);
        seat.preference[sizeof(seat.preference) - 1] = '\0';
    }
    else
    {
        strcpy(seat.preference, "Any");
    }

    fwrite(&seat, sizeof(Seat), 1, fp);
    fclose(fp);
    return 1;
}

int is_seat_booked(int seat_id, int bus_id)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return 0;

    Seat seat;
    fseek(fp, (seat_id - 1) * sizeof(Seat), SEEK_SET);
    if (fread(&seat, sizeof(Seat), 1, fp) == 1)
    {
        fclose(fp);
        return (seat.busId == bus_id && strcmp(seat.status, "Booked") == 0);
    }

    fclose(fp);
    return 0;
}

// ==========================================
// Matrix Preference Allocation & Auto Waitlist
// ==========================================

static int seat_matrix[TOTAL_ROWS][SEATS_PER_ROW] = {0}; // 0 = Free, StudentID = Booked

static bool is_window_seat(int col)
{
    return (col == 0 || col == 3);
}

static bool is_front_seat(int row)
{
    return (row == 0 || row == 1);
}

void init_seat_system(void)
{
    memset(seat_matrix, 0, sizeof(seat_matrix));
    init_storage_system();
}

int allocate_seat(int student_id, const char *name, const char *pref)
{
    int allocated_seat = -1;

    // 1. Preference Matching Pass
    for (int r = 0; r < TOTAL_ROWS; r++)
    {
        for (int c = 0; c < SEATS_PER_ROW; c++)
        {
            if (seat_matrix[r][c] == 0)
            {
                if (pref && strcmp(pref, "Window") == 0 && is_window_seat(c))
                {
                    allocated_seat = r * SEATS_PER_ROW + c + 1;
                    seat_matrix[r][c] = student_id;
                    break;
                }
                else if (pref && strcmp(pref, "Front") == 0 && is_front_seat(r))
                {
                    allocated_seat = r * SEATS_PER_ROW + c + 1;
                    seat_matrix[r][c] = student_id;
                    break;
                }
            }
        }
        if (allocated_seat != -1)
            break;
    }

    // 2. Fallback to ANY seat
    if (allocated_seat == -1)
    {
        for (int r = 0; r < TOTAL_ROWS; r++)
        {
            for (int c = 0; c < SEATS_PER_ROW; c++)
            {
                if (seat_matrix[r][c] == 0)
                {
                    allocated_seat = r * SEATS_PER_ROW + c + 1;
                    seat_matrix[r][c] = student_id;
                    break;
                }
            }
            if (allocated_seat != -1)
                break;
        }
    }

    // 3. Bus Full -> Push to Waitlist
    if (allocated_seat == -1)
    {
        Student st;
        st.studentId = student_id;
        strncpy(st.name, name, sizeof(st.name) - 1);
        st.name[sizeof(st.name) - 1] = '\0';
        save_waiting_student(&st);
        return 0;
    }

    // Sync with binary file
    book_seat_binary(allocated_seat, 101, student_id, pref);

    return allocated_seat;
}

bool cancel_seat(int student_id)
{
    for (int r = 0; r < TOTAL_ROWS; r++)
    {
        for (int c = 0; c < SEATS_PER_ROW; c++)
        {
            if (seat_matrix[r][c] == student_id)
            {
                seat_matrix[r][c] = 0;
                return true;
            }
        }
    }
    return false;
}

void display_bus_occupancy(void)
{
    printf("\n--- Bus Seat Occupancy Status ---\n");
    for (int r = 0; r < TOTAL_ROWS; r++)
    {
        printf("Row %2d: ", r + 1);
        for (int c = 0; c < SEATS_PER_ROW; c++)
        {
            printf("[%s] ", seat_matrix[r][c] == 0 ? "FREE" : "BUSY");
        }
        printf("\n");
    }
}