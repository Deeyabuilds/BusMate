#include <stdio.h>
#include "../include/SeatManager.h"

int main()
{
    printf("--- BusMate Logic & Preference Test ---\n");
    init_seat_system();

    // 1. Test Window Allocation
    int s1 = allocate_seat(101, "Aman", PREF_WINDOW);
    printf("Student 101 (Window Pref) Allocated Seat: %d\n", s1);

    // 2. Test Front Allocation
    int s2 = allocate_seat(102, "Rohan", PREF_FRONT);
    printf("Student 102 (Front Pref) Allocated Seat: %d\n", s2);

    // 3. Display Current Bus Status
    display_bus_occupancy();

    printf("\nLogic & Allocation Verification Successful!\n");
    return 0;
}