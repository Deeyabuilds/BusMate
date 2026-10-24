
#ifndef DRIVER_H
#define DRIVER_H

#include "structures.h"

#define MAX_DRIVERS 50

extern Driver drivers[MAX_DRIVERS];
extern int driverCount;

/* Existing Driver Data Management */
void loadDrivers(void);
int driverExists(int driverId);
int addDriver(int driverId, const char *name, const char *phone);
int removeDriver(int driverId);
void displayDrivers(void);

/* Driver Portal */
void driverMenu(void);
void viewDriverProfile(void);
void viewAssignedBus(void);
void viewAssignedRoute(void);
void viewDriverSchedule(void);
void updateBusStatus(void);
void viewSeatOccupancy(void);

#endif
