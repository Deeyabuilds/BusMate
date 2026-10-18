#ifndef DRIVER_H
#define DRIVER_H

#include "structures.h"

#define MAX_DRIVERS 50

extern Driver drivers[MAX_DRIVERS];
extern int driverCount;

void loadDrivers(void);
int driverExists(int driverId);
int addDriver(int driverId, const char *name, const char *phone);
int removeDriver(int driverId);
void displayDrivers(void);

#endif
