#ifndef ROUTE_H
#define ROUTE_H

#include "structures.h"

#define MAX_ROUTES 50

extern Route routes[MAX_ROUTES];
extern int routeCount;

void loadRoutes(void);
int routeExists(int routeId);
int addRoute(int routeId, const char *routeName, const char *stops);
int removeRoute(int routeId);
void displayRoutes(void);

#endif
