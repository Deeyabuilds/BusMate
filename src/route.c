#include <stdio.h>
#include <string.h>
#include "route.h"
#include "file_handler.h"

Route routes[MAX_ROUTES];
int routeCount = 0;

void loadRoutes(void)
{
    routeCount = 0;
    FILE *file = fopen("data/routes.dat", "rb");
    if (file == NULL)
        return;

    while (routeCount < MAX_ROUTES && fread(&routes[routeCount], sizeof(Route), 1, file) == 1)
    {
        routeCount++;
    }

    fclose(file);
}

int routeExists(int routeId)
{
    loadRoutes();
    for (int i = 0; i < routeCount; i++)
    {
        if (routes[i].routeId == routeId)
            return 1;
    }
    return 0;
}

int addRoute(int routeId, const char *routeName, const char *stops)
{
    loadRoutes();

    if (routeCount >= MAX_ROUTES)
    {
        printf("Route limit reached!\n");
        return -1;
    }

    if (routeExists(routeId))
    {
        printf("Route ID already exists!\n");
        return -2;
    }

    Route newRoute;
    newRoute.routeId = routeId;
    snprintf(newRoute.routeName, sizeof(newRoute.routeName), "%s", routeName ? routeName : "");
    snprintf(newRoute.stops, sizeof(newRoute.stops), "%s", stops ? stops : "");

    saveRoute(newRoute);
    loadRoutes();

    printf("Route added successfully!\n");
    return 0;
}

void displayRoutes(void)
{
    loadRoutes();

    if (routeCount == 0)
    {
        printf("\nNo routes available.\n");
        return;
    }

    printf("\n------ ROUTE LIST ------\n");

    for (int i = 0; i < routeCount; i++)
    {
        printf("\nRoute ID   : %d", routes[i].routeId);
        printf("\nRoute Name : %s", routes[i].routeName);
        printf("\nStops      : %s\n", routes[i].stops);
    }
}

int removeRoute(int routeId)
{
    loadRoutes();

    FILE *file = fopen("data/routes.dat", "rb");
    if (file == NULL)
    {
        printf("Unable to open route records.\n");
        return -1;
    }

    FILE *temp = fopen("data/temp_route.dat", "wb");
    if (temp == NULL)
    {
        fclose(file);
        printf("Unable to write temporary file.\n");
        return -1;
    }

    Route route;
    int found = 0;

    while (fread(&route, sizeof(Route), 1, file) == 1)
    {
        if (route.routeId == routeId)
        {
            found = 1;
            continue;
        }

        fwrite(&route, sizeof(Route), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove("data/routes.dat");
    rename("data/temp_route.dat", "data/routes.dat");

    loadRoutes();

    if (found)
    {
        printf("Route removed successfully!\n");
        return 0;
    }
    else
    {
        printf("Route ID not found!\n");
        return -1;
    }
}