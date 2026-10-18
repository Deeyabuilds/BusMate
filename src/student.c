#include <stdio.h>
#include <string.h>
#include "structures.h"

int studentLogin(Student *student)
{
    int id;
    char password[30];

    printf("\n--- Student Login ---\n");
    printf("Enter College ID: ");
    scanf("%d", &id);

    printf("Enter Password: ");
    scanf("%29s", password);

    if (student->studentId == id &&
        strcmp(student->password, password) == 0)
    {
        printf("\nLogin successful!\n");
        return 1;
    }

    printf("\nInvalid ID or password.\n");
    return 0;
}

int findBus(Bus buses[], int totalBuses, int busId)
{
    for (int i = 0; i < totalBuses; i++)
    {
        if (buses[i].busId == busId)
            return i;
    }

    return -1;
}

int findRoute(Route routes[], int totalRoutes, int routeId)
{
    for (int i = 0; i < totalRoutes; i++)
    {
        if (routes[i].routeId == routeId)
            return i;
    }

    return -1;
}

void availableBuses(Bus buses[], int totalBuses)
{
    printf("\n--- Available Buses ---\n");

    for (int i = 0; i < totalBuses; i++)
    {
        printf("Bus ID: %d | Number: %s | Available Seats: %d\n",
               buses[i].busId,
               buses[i].busNumber,
               buses[i].availableSeats);
    }
}

void searchBus(Bus buses[], int totalBuses)
{
    int busId;

    printf("\nEnter Bus ID to search: ");
    scanf("%d", &busId);

    int index = findBus(buses, totalBuses, busId);

    if (index == -1)
    {
        printf("Bus not found.\n");
        return;
    }

    printf("Bus ID: %d\n", buses[index].busId);
    printf("Bus Number: %s\n", buses[index].busNumber);
    printf("Capacity: %d\n", buses[index].capacity);
    printf("Available Seats: %d\n", buses[index].availableSeats);
}

void viewRouteTiming(Route routes[], int totalRoutes,
                     Schedule schedules[], int totalSchedules,
                     Bus buses[], int totalBuses)
{
    int busId;

    printf("\nEnter Bus ID: ");
    scanf("%d", &busId);

    int busIndex = findBus(buses, totalBuses, busId);

    if (busIndex == -1)
    {
        printf("Bus not found.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < totalSchedules; i++)
    {
        if (schedules[i].busId == busId)
        {
            int routeIndex = findRoute(
                routes, totalRoutes, schedules[i].routeId);

            if (routeIndex != -1)
            {
                printf("\nRoute: %s\n", routes[routeIndex].routeName);
                printf("Stops: %s\n", routes[routeIndex].stops);
                printf("Start Time: %s\n", schedules[i].startTime);
                printf("End Time: %s\n", schedules[i].endTime);
                printf("Day Type: %s\n", schedules[i].dayType);
                found = 1;
            }
        }
    }

    if (!found)
        printf("No schedule found for this bus.\n");
}

void checkSeatAvailability(Seat seats[], int totalSeats, int busId)
{
    int found = 0;

    printf("\n--- Seat Availability ---\n");

    for (int i = 0; i < totalSeats; i++)
    {
        if (seats[i].busId == busId &&
            strcmp(seats[i].status, "Available") == 0)
        {
            printf("Seat Number: %d\n", seats[i].seatNumber);
            found = 1;
        }
    }

    if (!found)
        printf("No available seats found for this bus.\n");
}

void bookSeat(Student *student, Seat seats[], int totalSeats)
{
    int busId, seatNumber;
    char preference[15];

    printf("\nEnter Bus ID: ");
    scanf("%d", &busId);

    checkSeatAvailability(seats, totalSeats, busId);

    printf("\nEnter Seat Number: ");
    scanf("%d", &seatNumber);

    printf("Preference (Front/Window/Any): ");
    scanf("%14s", preference);

    if (strcmp(preference, "Front") != 0 &&
        strcmp(preference, "Window") != 0 &&
        strcmp(preference, "Any") != 0)
    {
        printf("Invalid preference.\n");
        return;
    }

    for (int i = 0; i < totalSeats; i++)
    {
        if (seats[i].busId == busId &&
            seats[i].seatNumber == seatNumber)
        {
            if (strcmp(seats[i].status, "Available") != 0)
            {
                printf("Seat is not available.\n");
                return;
            }

            seats[i].studentId = student->studentId;
            strcpy(seats[i].status, "Booked");
            strcpy(seats[i].preference, preference);

            printf("Seat booked successfully!\n");
            return;
        }
    }

    printf("Seat not found for this bus.\n");
}

void studentDetails(Student *student, Seat seats[], int totalSeats)
{
    printf("\n--- My Booking ---\n");
    printf("Student ID: %d\n", student->studentId);
    printf("Name: %s\n", student->name);

    int found = 0;

    for (int i = 0; i < totalSeats; i++)
    {
        if (seats[i].studentId == student->studentId &&
            strcmp(seats[i].status, "Booked") == 0)
        {
            printf("Bus ID: %d\n", seats[i].busId);
            printf("Seat Number: %d\n", seats[i].seatNumber);
            printf("Preference: %s\n", seats[i].preference);
            found = 1;
        }
    }

    if (!found)
        printf("You have no current bookings.\n");
}

void cancelBooking(Student *student, Seat seats[], int totalSeats)
{
    int busId, seatNumber;

    printf("\nEnter Bus ID: ");
    scanf("%d", &busId);

    printf("Enter Seat Number: ");
    scanf("%d", &seatNumber);

    for (int i = 0; i < totalSeats; i++)
    {
        if (seats[i].busId == busId &&
            seats[i].seatNumber == seatNumber &&
            seats[i].studentId == student->studentId &&
            strcmp(seats[i].status, "Booked") == 0)
        {
            seats[i].studentId = 0;
            strcpy(seats[i].status, "Available");
            strcpy(seats[i].preference, "Any");

            printf("Booking cancelled successfully.\n");
            return;
        }
    }

    printf("No matching booking found.\n");
}

void studentModule(Student *student,
                   Bus buses[], int totalBuses,
                   Route routes[], int totalRoutes,
                   Schedule schedules[], int totalSchedules,
                   Seat seats[], int totalSeats)
{
    if (!studentLogin(student))
        return;

    int choice;

    do
    {
        printf("\n===== STUDENT DASHBOARD =====\n");
        printf("1. View Available Buses\n");
        printf("2. Search Bus\n");
        printf("3. View Routes and Timings\n");
        printf("4. Check Seat Availability\n");
        printf("5. Book Seat\n");
        printf("6. View My Booking\n");
        printf("7. Cancel Booking\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                availableBuses(buses, totalBuses);
                break;

            case 2:
                searchBus(buses, totalBuses);
                break;

            case 3:
                viewRouteTiming(routes, totalRoutes,
                                schedules, totalSchedules,
                                buses, totalBuses);
                break;

            case 4:
            {
                int busId;
                printf("Enter Bus ID: ");
                scanf("%d", &busId);

                if (findBus(buses, totalBuses, busId) == -1)
                    printf("Bus not found.\n");
                else
                    checkSeatAvailability(seats, totalSeats, busId);
                break;
            }

            case 5:
                bookSeat(student, seats, totalSeats);
                break;

            case 6:
                studentDetails(student, seats, totalSeats);
                break;

            case 7:
                cancelBooking(student, seats, totalSeats);
                break;

            case 0:
                printf("Logged out successfully.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}