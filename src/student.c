#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "file_handler.h"
#include "seat_allocation.h"

/* Student Module Functions */
void studentMenu(void);
void viewStudentProfile(void);
void searchBuses(void);
void viewStudentRoutes(void);
void viewStudentSchedule(void);
void checkSeatAvailability(void);
void bookSeat(void);
void cancelStudentBooking(void);
void viewWaitingList(void);

/* Read an integer safely */
int readInteger(const char *message, int *value)
{
    char input[100];
    char extra;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    if (sscanf(input, "%d %c", value, &extra) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        return 0;
    }

    return 1;
}

/* View Student Profile */
void viewStudentProfile(void)
{
    Student *students = NULL;
    size_t count = 0;
    size_t i;
    int studentId;
    int found = 0;

    if (!readInteger("Enter Student ID: ", &studentId))
        return;

    if (!readRecords(STUDENTS_FILE, sizeof(Student),
                     (void **)&students, &count))
    {
        printf("Unable to load student records.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (students[i].studentId == studentId)
        {
            printf("\n--- Student Profile ---\n");
            printf("Student ID: %d\n", students[i].studentId);
            printf("Name: %s\n", students[i].name);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found.\n");

    free(students);
}

/* Search Buses */
void searchBuses(void)
{
    Bus *buses = NULL;
    size_t count = 0;
    size_t i;
    int choice;
    int searchId;
    char busNumber[32];
    int found = 0;

    printf("\nSearch by:\n");
    printf("1. Bus ID\n");
    printf("2. Bus Number\n");

    if (!readInteger("Enter choice: ", &choice))
        return;

    if (!readRecords(BUSES_FILE, sizeof(Bus),
                     (void **)&buses, &count))
    {
        printf("Unable to load bus records.\n");
        return;
    }

    if (choice == 1)
    {
        if (!readInteger("Enter Bus ID: ", &searchId))
        {
            free(buses);
            return;
        }

        for (i = 0; i < count; i++)
        {
            if (buses[i].busId == searchId)
            {
                printf("\nBus ID: %d\n", buses[i].busId);
                printf("Bus Number: %s\n", buses[i].busNumber);
                printf("Capacity: %d\n", buses[i].capacity);
                printf("Available Seats: %d\n",
                       buses[i].availableSeats);

                found = 1;
                break;
            }
        }
    }
    else if (choice == 2)
    {
        printf("Enter Bus Number: ");

        if (scanf("%31s", busNumber) != 1)
        {
            free(buses);
            return;
        }

        for (i = 0; i < count; i++)
        {
            if (strcmp(buses[i].busNumber, busNumber) == 0)
            {
                printf("\nBus ID: %d\n", buses[i].busId);
                printf("Bus Number: %s\n", buses[i].busNumber);
                printf("Capacity: %d\n", buses[i].capacity);
                printf("Available Seats: %d\n",
                       buses[i].availableSeats);

                found = 1;
                break;
            }
        }
    }
    else
    {
        printf("Invalid choice.\n");
    }

    if (!found && (choice == 1 || choice == 2))
        printf("Bus not found.\n");

    free(buses);
}

/* View Routes */
void viewStudentRoutes(void)
{
    Route *routes = NULL;
    size_t count = 0;
    size_t i;

    if (!readRecords(ROUTES_FILE, sizeof(Route),
                     (void **)&routes, &count))
    {
        printf("Unable to load route records.\n");
        return;
    }

    printf("\n--- Available Routes ---\n");

    if (count == 0)
        printf("No routes available.\n");

    for (i = 0; i < count; i++)
    {
        printf("\nRoute ID: %d\n", routes[i].routeId);
        printf("Route Name: %s\n", routes[i].routeName);
        printf("Stops: %s\n", routes[i].stops);
    }

    free(routes);
}

/* View Student Schedule */
void viewStudentSchedule(void)
{
    Schedule *schedules = NULL;
    Bus *buses = NULL;
    Route *routes = NULL;

    size_t scheduleCount = 0;
    size_t busCount = 0;
    size_t routeCount = 0;

    size_t i, j;
    int busFound, routeFound;

    if (!readRecords(SCHEDULES_FILE, sizeof(Schedule),
                     (void **)&schedules, &scheduleCount))
    {
        printf("Unable to load schedules.\n");
        return;
    }

    if (!readRecords(BUSES_FILE, sizeof(Bus),
                     (void **)&buses, &busCount))
    {
        printf("Unable to load buses.\n");
        free(schedules);
        return;
    }

    if (!readRecords(ROUTES_FILE, sizeof(Route),
                     (void **)&routes, &routeCount))
    {
        printf("Unable to load routes.\n");
        free(schedules);
        free(buses);
        return;
    }

    printf("\n--- Bus Schedules ---\n");

    for (i = 0; i < scheduleCount; i++)
    {
        busFound = 0;
        routeFound = 0;

        printf("\nSchedule ID: %d\n",
               schedules[i].scheduleId);

        for (j = 0; j < busCount; j++)
        {
            if (buses[j].busId == schedules[i].busId)
            {
                printf("Bus ID: %d\n", buses[j].busId);
                printf("Bus Number: %s\n",
                       buses[j].busNumber);

                busFound = 1;
                break;
            }
        }

        for (j = 0; j < routeCount; j++)
        {
            if (routes[j].routeId == schedules[i].routeId)
            {
                printf("Route: %s\n",
                       routes[j].routeName);

                routeFound = 1;
                break;
            }
        }

        if (!busFound)
            printf("Bus details unavailable.\n");

        if (!routeFound)
            printf("Route details unavailable.\n");

        printf("Start Time: %s\n", schedules[i].startTime);
        printf("End Time: %s\n", schedules[i].endTime);
        printf("Day Type: %s\n", schedules[i].dayType);
    }

    free(schedules);
    free(buses);
    free(routes);
}

/* Check Seat Availability */
void checkSeatAvailability(void)
{
    int busId;

    if (!readInteger("Enter Bus ID: ", &busId))
        return;

    if (!displaySeatAvailability(busId, stdout))
        printf("Unable to display seat availability.\n");
}

/* Book a Seat */
void bookSeat(void)
{
    int studentId;
    int busId;
    int seatNumber;
    int choice;
    int result;

    char preference[16];
    Seat allocatedSeat;

    if (!readInteger("Enter Student ID: ", &studentId))
        return;

    if (!readInteger("Enter Bus ID: ", &busId))
        return;

    printf("\nSeat Preference:\n");
    printf("1. Window\n");
    printf("2. Front\n");
    printf("3. Any\n");

    if (!readInteger("Enter preference: ", &choice))
        return;

    switch (choice)
    {
        case 1:
            strcpy(preference, "Window");
            break;

        case 2:
            strcpy(preference, "Front");
            break;

        case 3:
            strcpy(preference, "Any");
            break;

        default:
            printf("Invalid preference.\n");
            return;
    }

    printf("\nBooking Options:\n");
    printf("1. Automatically allocate a seat\n");
    printf("2. Select a specific seat\n");

    if (!readInteger("Enter choice: ", &choice))
        return;

    if (choice == 1)
    {
        result = allocateSeat(busId, studentId,
                              preference, &allocatedSeat);
    }
    else if (choice == 2)
    {
        if (!readInteger("Enter Seat Number: ", &seatNumber))
            return;

        result = allocateSpecificSeat(
            busId, studentId, seatNumber,
            preference, &allocatedSeat);
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (result == SEAT_ALLOCATION_BOOKED)
    {
        printf("Seat booked successfully!\n");
        printf("Bus ID: %d\n", allocatedSeat.busId);
        printf("Seat Number: %d\n", allocatedSeat.seatNumber);
    }
    else if (result == SEAT_ALLOCATION_WAITLISTED)
    {
        printf("Bus is full. You have been added to the waiting list.\n");
    }
    else
    {
        printf("Booking failed. Please check the bus and seat details.\n");
    }
}

/* Cancel a Student Booking */
void cancelStudentBooking(void)
{
    int studentId;
    int busId;
    int seatNumber;
    int found = 0;
    int promotedStudentId = -1;

    size_t count = 0;
    size_t i;

    Seat *seats = NULL;
    Seat freedSeat;

    if (!readInteger("Enter Student ID: ", &studentId))
        return;

    if (!readInteger("Enter Bus ID: ", &busId))
        return;

    if (!readInteger("Enter Seat Number: ", &seatNumber))
        return;

    if (!getBusSeats(busId, &seats, &count))
    {
        printf("Unable to load bus seats.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (seats[i].busId == busId &&
            seats[i].seatNumber == seatNumber &&
            seats[i].studentId == studentId &&
            strcmp(seats[i].status, "Booked") == 0)
        {
            found = 1;
            break;
        }
    }

    free(seats);

    if (!found)
    {
        printf("No matching booking found for this seat.\n");
        return;
    }

    SeatCancellationResult result =
        cancelSeat(busId, studentId, &freedSeat,
                   &promotedStudentId);

    if (result == SEAT_CANCELLATION_PROMOTED)
    {
        printf("Booking cancelled successfully.\n");
        printf("Student %d was promoted from the waiting list.\n",
               promotedStudentId);
    }
    else if (result == SEAT_CANCELLATION_COMPLETED)
    {
        printf("Booking cancelled successfully.\n");
    }
    else
    {
        printf("Cancellation failed.\n");
    }
}

/* View Waiting List */
void viewWaitingList(void)
{
    int busId;

    if (!readInteger("Enter Bus ID: ", &busId))
        return;

    if (!displayWaitingList(busId, stdout))
        printf("Unable to display waiting list.\n");
}

/* Student Menu */
void studentMenu(void)
{
    int choice;

    do
    {
        printf("\n========== STUDENT MENU ==========\n");
        printf("1. View Student Profile\n");
        printf("2. Search Buses\n");
        printf("3. View Routes\n");
        printf("4. View Bus Schedules\n");
        printf("5. Check Seat Availability\n");
        printf("6. Book a Seat\n");
        printf("7. Cancel Booking\n");
        printf("8. View Waiting List\n");
        printf("0. Exit Student Menu\n");

        if (!readInteger("Enter your choice: ", &choice))
            continue;

        switch (choice)
        {
            case 1:
                viewStudentProfile();
                break;

            case 2:
                searchBuses();
                break;

            case 3:
                viewStudentRoutes();
                break;

            case 4:
                viewStudentSchedule();
                break;

            case 5:
                checkSeatAvailability();
                break;

            case 6:
                bookSeat();
                break;

            case 7:
                cancelStudentBooking();
                break;

            case 8:
                viewWaitingList();
                break;

            case 0:
                printf("Exiting Student Menu...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 0);
}