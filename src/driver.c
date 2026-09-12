#include <stdio.h>
#include "Driver.h"

#define MAX_DRIVERS 50

Driver drivers[MAX_DRIVERS];
int driverCount = 0;

void addDriver()
{
    if (driverCount >= MAX_DRIVERS)
    {
        printf("Driver limit reached!\n");
        return;
    }

    printf("\nEnter Driver ID: ");
    scanf("%d", &drivers[driverCount].driverId);

    printf("Enter Driver Name: ");
    scanf(" %[^]", drivers[driverCount].name);

    printf("Enter Driver Phone: ");
    scanf("%s", drivers[driverCount].phone);

    driverCount++;

    printf("\nDriver added successfully!\n");
}

void displayDrivers()
{
    if (driverCount == 0)
    {
        printf("\nNo drivers available.\n");
        return;
    }

    printf("\n------ DRIVER LIST ------\n");

    for (int i = 0; i < driverCount; i++)
    {
        printf("\nDriver ID: %d", drivers[i].driverId);
        printf("\nName: %s", drivers[i].name);
        printf("\nPhone: %s\n", drivers[i].phone);
    }
}
