#include <stdio.h>
#include <string.h>
#include "driver.h"
#include "file_handler.h"

Driver drivers[MAX_DRIVERS];
int driverCount = 0;

void loadDrivers(void)
{
    driverCount = 0;
    FILE *file = fopen("data/drivers.dat", "rb");
    if (file == NULL)
        return;

    while (driverCount < MAX_DRIVERS && fread(&drivers[driverCount], sizeof(Driver), 1, file) == 1)
    {
        driverCount++;
    }

    fclose(file);
}

int driverExists(int driverId)
{
    loadDrivers();
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i].driverId == driverId)
            return 1;
    }
    return 0;
}

int addDriver(int driverId, const char *name, const char *phone)
{
    loadDrivers();

    if (driverCount >= MAX_DRIVERS)
    {
        printf("Driver limit reached!\n");
        return -1;
    }

    if (driverExists(driverId))
    {
        printf("Driver ID already exists!\n");
        return -2;
    }

    Driver newDriver;
    newDriver.driverId = driverId;
    snprintf(newDriver.name, sizeof(newDriver.name), "%s", name ? name : "");
    snprintf(newDriver.phone, sizeof(newDriver.phone), "%s", phone ? phone : "");

    saveDriver(newDriver);
    loadDrivers();

    printf("Driver added successfully!\n");
    return 0;
}

void displayDrivers(void)
{
    loadDrivers();

    if (driverCount == 0)
    {
        printf("\nNo drivers available.\n");
        return;
    }

    printf("\n------ DRIVER LIST ------\n");

    for (int i = 0; i < driverCount; i++)
    {
        printf("\nDriver ID: %d", drivers[i].driverId);
        printf("\nName     : %s", drivers[i].name);
        printf("\nPhone    : %s\n", drivers[i].phone);
    }
}

int removeDriver(int driverId)
{
    loadDrivers();

    FILE *file = fopen("data/drivers.dat", "rb");
    if (file == NULL)
    {
        printf("Unable to open driver records.\n");
        return -1;
    }

    FILE *temp = fopen("data/temp_driver.dat", "wb");
    if (temp == NULL)
    {
        fclose(file);
        printf("Unable to write temporary file.\n");
        return -1;
    }

    Driver driver;
    int found = 0;

    while (fread(&driver, sizeof(Driver), 1, file) == 1)
    {
        if (driver.driverId == driverId)
        {
            found = 1;
            continue;
        }

        fwrite(&driver, sizeof(Driver), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove("data/drivers.dat");
    rename("data/temp_driver.dat", "data/drivers.dat");

    loadDrivers();

    if (found)
    {
        printf("Driver removed successfully!\n");
        return 0;
    }
    else
    {
        printf("Driver ID not found!\n");
        return -1;
    }
}