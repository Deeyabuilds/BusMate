
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

    while (routeCount < MAX_ROUTES &&
           fread(&routes[routeCount], sizeof(Route), 1, file) == 1)
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

    for (int i = 0; i < routeCount; i++)
    {
        if (routes[i].routeId == routeId)
        {
            printf("Route ID already exists!\n");
            return -2;
        }
    }

    Route newRoute;
    newRoute.routeId = routeId;

    snprintf(newRoute.routeName, sizeof(newRoute.routeName),
             "%s", routeName ? routeName : "");

    snprintf(newRoute.stops, sizeof(newRoute.stops),
             "%s", stops ? stops : "");

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

    int found = 0;
    int newCount = 0;

    for (int i = 0; i < routeCount; i++)
    {
        if (routes[i].routeId == routeId)
        {
            found = 1;
            continue;
        }

        routes[newCount++] = routes[i];
    }

    if (!found)
    {
        printf("Route ID not found!\n");
        return -1;
    }

    FILE *file = fopen("data/routes.dat", "wb");
    if (file == NULL)
    {
        printf("Unable to open route records.\n");
        return -1;
    }

    size_t saved = fwrite(routes, sizeof(Route), newCount, file);
    fclose(file);

    if (saved != (size_t)newCount)
    {
        printf("Error saving route records!\n");
        return -1;
    }

    routeCount = newCount;
    printf("Route removed successfully!\n");
    return 0;
}

int editRoute(int routeId, const char *routeName, const char *stops)
{
    if (routeName == NULL || stops == NULL)
    {
        printf("Invalid route details!\n");
        return -1;
    }

    loadRoutes();

    for (int i = 0; i < routeCount; i++)
    {
        if (routes[i].routeId == routeId)
        {
            snprintf(routes[i].routeName,
                     sizeof(routes[i].routeName), "%s", routeName);

            snprintf(routes[i].stops,
                     sizeof(routes[i].stops), "%s", stops);

            FILE *file = fopen("data/routes.dat", "wb");
            if (file == NULL)
            {
                printf("Error opening routes file!\n");
                return -1;
            }

            size_t saved = fwrite(routes, sizeof(Route), routeCount, file);
            fclose(file);

            if (saved != (size_t)routeCount)
            {
                printf("Error saving updated route!\n");
                return -1;
            }

            printf("Route updated successfully!\n");
            return 0;
        }
    }

    printf("Route not found!\n");
    return -1;
}
