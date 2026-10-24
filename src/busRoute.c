
#include <stdio.h>
#include <string.h>
#include "busRoute.h"
#include "bus.h"
#include "route.h"

void busModuleAddBus(void)
{
    int id, capacity;
    char number[50];

    printf("Enter Bus ID: ");
    scanf("%d", &id);
    printf("Enter Bus Number: ");
    scanf("%49s", number);
    printf("Enter Bus Capacity: ");
    scanf("%d", &capacity);

    if (capacity <= 0) {
        printf("Capacity must be positive.\n");
        return;
    }

    addBus(id, number, capacity);
}

void busModuleViewBuses(void)
{
    displayBuses();
}

void busModuleSearchBuses(void)
{
    int id;
    printf("Enter Bus ID to search: ");
    scanf("%d", &id);

    if (busExists(id))
        printf("Bus ID %d exists.\n", id);
    else
        printf("Bus not found.\n");
}

void busModuleUpdateBus(void)
{
    printf("Bus update is not supported by the existing bus module yet.\n");
}

void busModuleRemoveBus(void)
{
    int id;
    printf("Enter Bus ID to remove: ");
    scanf("%d", &id);
    removeBus(id);
}

void busModuleAddRoute(void)
{
    int id;
    char name[100], stops[300];

    printf("Enter Route ID: ");
    scanf("%d", &id);
    getchar();

    printf("Enter Route Name: ");
    if (!fgets(name, sizeof(name), stdin)) return;
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Stops (comma-separated): ");
    if (!fgets(stops, sizeof(stops), stdin)) return;
    stops[strcspn(stops, "\n")] = '\0';

    addRoute(id, name, stops);
}

void busModuleViewRoutes(void)
{
    displayRoutes();
}

void busModuleSearchRoutes(void)
{
    int id;
    printf("Enter Route ID to search: ");
    scanf("%d", &id);

    if (routeExists(id))
        printf("Route ID %d exists.\n", id);
    else
        printf("Route not found.\n");
}

void busModuleUpdateRoute(void)
{
    printf("Route update is not supported by the existing route module yet.\n");
}

void busModuleRemoveRoute(void)
{
    int id;
    printf("Enter Route ID to remove: ");
    scanf("%d", &id);
    removeRoute(id);
}

void busRouteMenu(void)
{
    int choice;

    do {
        printf("\n--- BUS AND ROUTE MANAGEMENT ---\n");
        printf("1. Add Bus\n");
        printf("2. View Buses\n");
        printf("3. Search Bus\n");
        printf("4. Update Bus\n");
        printf("5. Remove Bus\n");
        printf("6. Add Route\n");
        printf("7. View Routes\n");
        printf("8. Search Route\n");
        printf("9. Update Route\n");
        printf("10. Remove Route\n");
        printf("0. Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n' && !feof(stdin)) {}
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: busModuleAddBus(); break;
            case 2: busModuleViewBuses(); break;
            case 3: busModuleSearchBuses(); break;
            case 4: busModuleUpdateBus(); break;
            case 5: busModuleRemoveBus(); break;
            case 6: busModuleAddRoute(); break;
            case 7: busModuleViewRoutes(); break;
            case 8: busModuleSearchRoutes(); break;
            case 9: busModuleUpdateRoute(); break;
            case 10: busModuleRemoveRoute(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}
