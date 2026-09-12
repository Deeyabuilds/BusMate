#include <stdio.h>
#include "route.h"

#define MAX_ROUTES 50

Route routes[MAX_ROUTES];
int routeCount = 0;

void addRoute()
{
    if (routeCount >= MAX_ROUTES)
    {
        printf("Route limit reached!\n");
        return;
    }

    printf("\nEnter Route ID: ");
    scanf("%d", &routes[routeCount].routeId);

    printf("Enter Route Name: ");
    scanf(" %[^\n]", routes[routeCount].routeName);

    printf("Enter Stops: ");
    scanf(" %[^\n]", routes[routeCount].stops);

    routeCount++;

    printf("\nRoute added successfully!\n");
}

void displayRoutes()
{
    if (routeCount == 0)
    {
        printf("\nNo routes available.\n");
        return;
    }

    printf("\n------ ROUTE LIST ------\n");

    for (int i = 0; i < routeCount; i++)
    {
        printf("\nRoute ID: %d", routes[i].routeId);
        printf("\nRoute Name: %s", routes[i].routeName);
        printf("\nStops: %s\n", routes[i].stops);
    }
}
