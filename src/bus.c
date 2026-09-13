#include <stdio.h>
#include "bus.h"

#define MAX_BUSES 50

Bus buses[MAX_BUSES];
int busCount = 0;

void addBus()
{
    if (busCount >= MAX_BUSES)
    {
        printf("Bus limit reached!\n");
        return;
    }

    printf("\nEnter Bus ID: ");
    scanf("%d", &buses[busCount].busId);

    printf("Enter Bus Number: ");
    scanf("%s", buses[busCount].busNumber);

    printf("Enter Bus Capacity: ");
    scanf("%d", &buses[busCount].capacity);

    buses[busCount].availableSeats = buses[busCount].capacity;

    busCount++;

    printf("\nBus added successfully!\n");
}

void displayBuses()
{
    if (busCount == 0)
    {
        printf("\nNo buses available.\n");
        return;
    }

    printf("\n------ BUS LIST ------\n");

    for (int i = 0; i < busCount; i++)
    {
        printf("\nBus ID: %d", buses[i].busId);
        printf("\nBus Number: %s", buses[i].busNumber);
        printf("\nCapacity: %d", buses[i].capacity);
        printf("\nAvailable Seats: %d\n", buses[i].availableSeats);
    }
}