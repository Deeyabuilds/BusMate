#include <stdio.h>
#include <string.h>
#include "student.h"
#include "structures.h"

#define TOTAL_BUSES 3
#define SEATS_PER_BUS 20

/* Student data */
Student student = {45, "Smera Tewari", "1234"};

/* Bus data */
Bus buses[TOTAL_BUSES] = {
    {1, "BUS-101", 20, 20},
    {2, "BUS-102", 20, 20},
    {3, "BUS-103", 20, 20}
};

/* Route data */
Route routes[TOTAL_BUSES] = {
    {1, "Nainital - Haldwani",
     "Tallital, Bhowali, Kathgodam, Haldwani"},

    {2, "Nainital - Bhowali",
     "Tallital, Bhowali"},

    {3, "Nainital - Kathgodam",
     "Tallital, Bhowali, Kathgodam"}
};

/* Schedule data */
Schedule schedules[TOTAL_BUSES] = {
    {1, 1, 1, "08:00 AM", "09:30 AM", "Full Day"},
    {2, 2, 2, "09:00 AM", "10:00 AM", "Full Day"},
    {3, 3, 3, "10:00 AM", "11:30 AM", "Half Day"}
};

/* Seat data */
Seat seats[TOTAL_BUSES * SEATS_PER_BUS];


/* Initialize all seats as available */
void initializeSeats()
{
    int index = 0;

    for(int bus = 0; bus < TOTAL_BUSES; bus++)
    {
        for(int seat = 1; seat <= SEATS_PER_BUS; seat++)
        {
            seats[index].seatNumber = seat;
            seats[index].busId = bus + 1;
            seats[index].studentId = 0;

            strcpy(seats[index].status, "Available");
            strcpy(seats[index].preference, "Any");

            index++;
        }
    }
}


/* Student login */
int studentLogin()
{
    int id;
    char password[30];

    printf("\n===== STUDENT LOGIN =====\n");

    printf("Enter College ID: ");
    scanf("%d", &id);

    printf("Enter Password: ");
    scanf("%s", password);

    if(id == student.studentId &&
       strcmp(password, student.password) == 0)
    {
        printf("\nLogin successful!\n");
        printf("Welcome, %s!\n", student.name);

        return 1;
    }

    printf("\nInvalid College ID or Password.\n");

    return 0;
}


/* View all available buses */
void availableBuses()
{
    printf("\n===== AVAILABLE BUSES =====\n");

    for(int i = 0; i < TOTAL_BUSES; i++)
    {
        printf("\nBus Number: %s\n", buses[i].busNumber);
        printf("Route: %s\n", routes[i].routeName);
        printf("Available Seats: %d\n",
               buses[i].availableSeats);
        printf("Day Type: %s\n",
               schedules[i].dayType);
    }
}


/* Search bus or route */
void searchBus()
{
    int choice;
    int busId;
    char routeName[50];

    printf("\n===== BUS / ROUTE SEARCH =====\n");

    printf("1. Search by Bus ID\n");
    printf("2. Search by Route\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        printf("Enter Bus ID: ");
        scanf("%d", &busId);

        for(int i = 0; i < TOTAL_BUSES; i++)
        {
            if(buses[i].busId == busId)
            {
                printf("\nBus Number: %s\n",
                       buses[i].busNumber);

                printf("Route: %s\n",
                       routes[i].routeName);

                printf("Stops: %s\n",
                       routes[i].stops);

                printf("Timing: %s - %s\n",
                       schedules[i].startTime,
                       schedules[i].endTime);

                printf("Day Type: %s\n",
                       schedules[i].dayType);

                printf("Available Seats: %d\n",
                       buses[i].availableSeats);

                return;
            }
        }

        printf("Bus not found.\n");
    }
    else if(choice == 2)
    {
        printf("Enter Route Name: ");
        scanf(" %[^\n]", routeName);

        for(int i = 0; i < TOTAL_BUSES; i++)
        {
            if(strcmp(routes[i].routeName, routeName) == 0)
            {
                printf("\nBus Number: %s\n",
                       buses[i].busNumber);

                printf("Route: %s\n",
                       routes[i].routeName);

                printf("Stops: %s\n",
                       routes[i].stops);

                printf("Timing: %s - %s\n",
                       schedules[i].startTime,
                       schedules[i].endTime);

                printf("Day Type: %s\n",
                       schedules[i].dayType);

                printf("Available Seats: %d\n",
                       buses[i].availableSeats);

                return;
            }
        }

        printf("Route not found.\n");
    }
    else
    {
        printf("Invalid choice.\n");
    }
}


/* View routes, stops and timings */
void viewRouteTiming()
{
    printf("\n===== ROUTES AND TIMINGS =====\n");

    for(int i = 0; i < TOTAL_BUSES; i++)
    {
        printf("\nBus: %s\n",
               buses[i].busNumber);

        printf("Route: %s\n",
               routes[i].routeName);

        printf("Stops: %s\n",
               routes[i].stops);

        printf("Start Time: %s\n",
               schedules[i].startTime);

        printf("End Time: %s\n",
               schedules[i].endTime);

        printf("Day Type: %s\n",
               schedules[i].dayType);
    }
}


/* Check available seats of a bus */
void checkSeatAvailability()
{
    int busId;

    printf("\n===== SEAT AVAILABILITY =====\n");

    printf("Enter Bus ID: ");
    scanf("%d", &busId);

    for(int i = 0; i < TOTAL_BUSES; i++)
    {
        if(buses[i].busId == busId)
        {
            printf("Bus Number: %s\n",
                   buses[i].busNumber);

            printf("Available Seats: %d\n",
                   buses[i].availableSeats);

            return;
        }
    }

    printf("Bus not found.\n");
}


/* Book a seat */
void bookSeat()
{
    int busId;
    int seatNumber;
    char preference[15];

    printf("\n===== BOOK SEAT =====\n");

    printf("Enter Bus ID: ");
    scanf("%d", &busId);

    printf("Enter Seat Number (1-%d): ",
           SEATS_PER_BUS);
    scanf("%d", &seatNumber);

    if(seatNumber < 1 ||
       seatNumber > SEATS_PER_BUS)
    {
        printf("Invalid seat number.\n");
        return;
    }

    for(int i = 0;
        i < TOTAL_BUSES * SEATS_PER_BUS;
        i++)
    {
        if(seats[i].busId == busId &&
           seats[i].seatNumber == seatNumber)
        {
            if(strcmp(seats[i].status, "Booked") == 0)
            {
                printf("Seat already booked.\n");
                return;
            }

            printf("Enter preference (Window/Front/Any): ");
            scanf("%s", preference);

            seats[i].studentId = student.studentId;

            strcpy(seats[i].status, "Booked");
            strcpy(seats[i].preference, preference);

            for(int j = 0; j < TOTAL_BUSES; j++)
            {
                if(buses[j].busId == busId)
                {
                    buses[j].availableSeats--;
                    break;
                }
            }

            printf("\nSeat booked successfully!\n");

            printf("Bus: %s\n",
                   buses[busId - 1].busNumber);

            printf("Seat Number: %d\n",
                   seatNumber);

            printf("Preference: %s\n",
                   preference);

            return;
        }
    }

    printf("Seat not found.\n");
}


/* View student's own seat */
void studentDetails()
{
    int found = 0;

    printf("\n===== MY DETAILS =====\n");

    printf("College ID: %d\n",
           student.studentId);

    printf("Name: %s\n",
           student.name);

    printf("\nMy Seat:\n");

    for(int i = 0;
        i < TOTAL_BUSES * SEATS_PER_BUS;
        i++)
    {
        if(seats[i].studentId == student.studentId &&
           strcmp(seats[i].status, "Booked") == 0)
        {
            printf("Bus ID: %d\n",
                   seats[i].busId);

            printf("Seat Number: %d\n",
                   seats[i].seatNumber);

            printf("Preference: %s\n",
                   seats[i].preference);

            found = 1;
        }
    }

    if(found == 0)
    {
        printf("No seat booked currently.\n");
    }
}


/* Cancel student's seat */
void cancelBooking()
{
    int seatNumber;

    printf("\n===== CANCEL MY SEAT =====\n");

    printf("Enter Seat Number: ");
    scanf("%d", &seatNumber);

    for(int i = 0;
        i < TOTAL_BUSES * SEATS_PER_BUS;
        i++)
    {
        if(seats[i].seatNumber == seatNumber &&
           seats[i].studentId == student.studentId &&
           strcmp(seats[i].status, "Booked") == 0)
        {
            int busId = seats[i].busId;

            seats[i].studentId = 0;

            strcpy(seats[i].status, "Available");
            strcpy(seats[i].preference, "Any");

            for(int j = 0; j < TOTAL_BUSES; j++)
            {
                if(buses[j].busId == busId)
                {
                    buses[j].availableSeats++;
                    break;
                }
            }

            printf("Seat cancelled successfully.\n");

            return;
        }
    }

    printf("No booking found for this seat.\n");
}


/* Student dashboard */
void studentMenu()
{
    int choice;

    do
    {
        printf("\n================================\n");
        printf("        STUDENT DASHBOARD\n");
        printf("================================\n");

        printf("1. View Available Buses\n");
        printf("2. Search Bus / Route\n");
        printf("3. View Routes and Timings\n");
        printf("4. Check Seat Availability\n");
        printf("5. Book Seat\n");
        printf("6. View My Seat\n");
        printf("7. Cancel My Seat\n");
        printf("8. Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                availableBuses();
                break;

            case 2:
                searchBus();
                break;

            case 3:
                viewRouteTiming();
                break;

            case 4:
                checkSeatAvailability();
                break;

            case 5:
                bookSeat();
                break;

            case 6:
                studentDetails();
                break;

            case 7:
                cancelBooking();
                break;

            case 8:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while(choice != 8);
}


/* Start Student Module */
void studentModule()
{
    initializeSeats();

    if(studentLogin())
    {
        studentMenu();
    }
}