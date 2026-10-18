#include <stdio.h>
#include <string.h>
#include "bus.h"
#include "file_handler.h"

Bus buses[MAX_BUSES];
int busCount = 0;

void loadBuses(void)
{
    busCount = 0;
    FILE *file = fopen("data/buses.dat", "rb");
    if (file == NULL)
        return;

    while (busCount < MAX_BUSES && fread(&buses[busCount], sizeof(Bus), 1, file) == 1)
    {
        busCount++;
    }

    fclose(file);
}

int busExists(int busId)
{
    loadBuses();
    for (int i = 0; i < busCount; i++)
    {
        if (buses[i].busId == busId)
            return 1;
    }
    return 0;
}

int addBus(int busId, const char *busNumber, int capacity)
{
    loadBuses();

    if (busCount >= MAX_BUSES)
    {
        printf("Bus limit reached!\n");
        return -1;
    }

    if (busExists(busId))
    {
        printf("Bus ID already exists!\n");
        return -2;
    }

    Bus newBus;
    newBus.busId = busId;
    snprintf(newBus.busNumber, sizeof(newBus.busNumber), "%s", busNumber ? busNumber : "");
    newBus.capacity = capacity;
    newBus.availableSeats = capacity;

    saveBus(newBus);
    loadBuses();

    printf("Bus added successfully!\n");
    return 0;
}

void displayBuses(void)
{
    loadBuses();

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

int removeBus(int busId)
{
    loadBuses();

    FILE *file = fopen("data/buses.dat", "rb");
    if (file == NULL)
    {
        printf("Unable to open bus records.\n");
        return -1;
    }

    FILE *temp = fopen("data/temp_bus.dat", "wb");
    if (temp == NULL)
    {
        fclose(file);
        printf("Unable to write temporary file.\n");
        return -1;
    }

    Bus bus;
    int found = 0;

    while (fread(&bus, sizeof(Bus), 1, file) == 1)
    {
        if (bus.busId == busId)
        {
            found = 1;
            continue;
        }

        fwrite(&bus, sizeof(Bus), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove("data/buses.dat");
    rename("data/temp_bus.dat", "data/buses.dat");

    loadBuses();

    if (found)
    {
        printf("Bus removed successfully!\n");
        return 0;
    }
    else
    {
        printf("Bus ID not found!\n");
        return -1;
    }
}