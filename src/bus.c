#include <stdio.h>
#include <string.h>
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
    strcpy(buses[busCount].status,"Not Started");

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
        printf("\nDriver ID: %d\n", buses[i].driverId);
        printf("\nStatus:%s\n", buses[i].status);
    }
}
#define MAX_SCHEDULES 50

Schedule schedules[MAX_SCHEDULES];
int scheduleCount = 0;

void addSchedule()
{
    if (scheduleCount >= MAX_SCHEDULES)
    {
        printf("Schedule limit reached!\n");
        return;
    }

    printf("\nEnter Schedule ID: ");
    scanf("%d", &schedules[scheduleCount].scheduleId);

    printf("Enter Bus ID: ");
    scanf("%d", &schedules[scheduleCount].busId);

    printf("Enter Route ID: ");
    scanf("%d", &schedules[scheduleCount].routeId);

    printf("Enter Start Time: ");
    scanf("%s", schedules[scheduleCount].startTime);

    printf("Enter End Time: ");
    scanf("%s", schedules[scheduleCount].endTime);

    printf("Enter Day Type (Full Day/Half Day): ");
    scanf(" %[^\n]", schedules[scheduleCount].dayType);

    scheduleCount++;

    printf("\nSchedule added successfully!\n");
}

void displaySchedules()
{
    if (scheduleCount == 0)
    {
        printf("\nNo schedules available.\n");
        return;
    }

    printf("\n------ BUS SCHEDULES ------\n");

    for (int i = 0; i < scheduleCount; i++)
    {
        printf("\nSchedule ID: %d", schedules[i].scheduleId);
        printf("\nBus ID: %d", schedules[i].busId);
        printf("\nRoute ID: %d", schedules[i].routeId);
        printf("\nStart Time: %s", schedules[i].startTime);
        printf("\nEnd Time: %s", schedules[i].endTime);
        printf("\nDay Type: %s\n", schedules[i].dayType);
    }
}
void assignDriverToBus()
{
    int busId, driverId;
    int found = 0;

    printf("\nEnter Bus ID: ");
    scanf("%d", &busId);

    printf("Enter Driver ID: ");
    scanf("%d", &driverId);

    for (int i = 0; i < busCount; i++)
    {
        if (buses[i].busId == busId)
        {
            buses[i].driverId = driverId;
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("\nDriver assigned to bus successfully!\n");
    }
    else
    {
        printf("\nBus not found!\n");
    }
}
void updateBusStatus()
{
    int busId;
    int choice;
    int found = 0;

    printf("\nEnter Bus ID: ");
    scanf("%d", &busId);

    for (int i = 0; i < busCount; i++)
    {
        if (buses[i].busId == busId)
        {
            found = 1;

            printf("\nSelect Bus Status:");
            printf("\n1. Not Started");
            printf("\n2. Running");
            printf("\n3. Completed");
            printf("\nEnter choice: ");
            scanf("%d", &choice);

            if (choice == 1)
                strcpy(buses[i].status, "Not Started");
            else if (choice == 2)
                strcpy(buses[i].status, "Running");
            else if (choice == 3)
                strcpy(buses[i].status, "Completed");
            else
            {
                printf("\nInvalid choice!\n");
                return;
            }

            printf("\nBus status updated successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("\nBus not found!\n");
    }
}