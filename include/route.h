
#ifndef ROUTE_H
#define ROUTE_H

#include "structures.h"

#define MAX_ROUTES 100

extern Route routes[MAX_ROUTES];
extern int routeCount;

void loadRoutes(void);
int routeExists(int routeId);
int addRoute(int routeId, const char *routeName, const char *stops);
void displayRoutes(void);
int removeRoute(int routeId);

/* Update an existing route */
int editRoute(int routeId, const char *routeName, const char *stops);

#endif
