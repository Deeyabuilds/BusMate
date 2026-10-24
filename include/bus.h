#ifndef BUS_H
#define BUS_H

#include "structures.h"

#define MAX_BUSES 50

extern Bus buses[MAX_BUSES];
extern int busCount;

void loadBuses(void);
int busExists(int busId);
int addBus(int busId, const char *busNumber, int capacity);
int removeBus(int busId);
void displayBuses(void);

#endif