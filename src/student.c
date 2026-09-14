#include <stdio.h>
#include <string.h>
#include "student.h"
#include "structures.h"

#define TOTAL_BUSES 3
#define SEATS_PER_BUS 20

Student student = {45, "Smera Tewari", "1234"};

Bus buses[TOTAL_BUSES] = {
    {1, "BUS-101", 20, 20},
    {2, "BUS-102", 20, 20},
    {3, "BUS-103", 20, 20}
};

Route routes[TOTAL_BUSES] = {
    {1, "Nainital - Haldwani",
     "Tallital, Bhowali, Kathgodam, Haldwani"},

    {2, "Nainital - Bhowali",
     "Tallital, Bhowali"},

    {3, "Nainital - Kathgodam",
     "Tallital, Bhowali, Kathgodam"}
};

Schedule schedules[TOTAL_BUSES] = {
    {1, 1, 1, "08:00 AM", "09:30 AM", "Full Day"},
    {2, 2, 2, "09:00 AM", "10:00 AM", "Full Day"},
    {3, 3, 3, "10:00 AM", "11:30 AM", "Half Day"}
};

Seat seats[TOTAL_BUSES * SEATS_PER_BUS];


/* Initialize seats */
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


/* Student Login */
int studentLogin()
{
    int id;
    char password[30];

    printf("\n===== STUDENT LOGIN =====\n");

    printf("Enter Student ID: ");
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

    printf("\nInvalid Student ID or Password.\n");
    return 0;
}


/* Search Bus */
void searchBus()
{
    int busId;

    printf("\n===== SEARCH BUS =====\n");
    printf("Enter Bus ID: ");
    scanf("%d", &busId);

    for(int i = 0; i < TOTAL_BUSES; i++)
    {
        if(buses[i].busId == busId)
        {
            printf("\nBus Number: %s\n", buses[i].busNumber);
            printf("Route: %s\n", routes[i].routeName);
            printf("Available Seats: %d\n",
                   buses[i].availableSeats);

            return;
        }
    }

    printf("Bus not found.\n");
}


/* View Route and Timing */
void viewRouteTiming()
{
    printf("\n===== ROUTE AND TIMING =====\n");

    for(int i = 0; i < TOTAL_BUSES; i++)
    {
        printf("\nBus: %s\n", buses[i].busNumber);
        printf("Route: %s\n", routes[i].routeName);
        printf("Stops: %s\n", routes[i].stops);
        printf("Start Time: %s\n", schedules[i].startTime);
        printf("End Time: %s\n", schedules[i].endTime);
    }
}


/* Check Seat Availability */
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
            printf("Available Seats: %d\n",
                   buses[i].availableSeats);

            return;
        }
    }

    printf("Bus not found.\n");
}


/* Book Seat */
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

    if(seatNumber < 1 || seatNumber > SEATS_PER_BUS)
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
            printf("Bus: %s\n", buses[busId - 1].busNumber);
            printf("Seat Number: %d\n", seatNumber);
            printf("Preference: %s\n", preference);

            return;
        }
    }

    printf("Seat not found.\n");
}


/* Cancel Booking */
void cancelBooking()
{
    int seatNumber;

    printf("\n===== CANCEL BOOKING =====\n");

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

            printf("Booking cancelled successfully.\n");
            return;
        }
    }

    printf("No booking found for this seat.\n");
}


/* Student Details */
void studentDetails()
{
    int found = 0;

    printf("\n===== STUDENT DETAILS =====\n");

    printf("Student ID: %d\n", student.studentId);
    printf("Name: %s\n", student.name);

    printf("\nCurrent Booking:\n");

    for(int i = 0;
        i < TOTAL_BUSES * SEATS_PER_BUS;
        i++)
    {
        if(seats[i].studentId == student.studentId &&
           strcmp(seats[i].status, "Booked") == 0)
        {
            printf("Bus ID: %d\n", seats[i].busId);
            printf("Seat Number: %d\n", seats[i].seatNumber);
            printf("Preference: %s\n", seats[i].preference);

            found = 1;
        }
    }

    if(found == 0)
    {
        printf("No current booking.\n");
    }
}


/* Student Menu */
void studentMenu()
{
    int choice;

    do
    {
        printf("\n===== STUDENT MODULE =====\n");

        printf("1. Search Bus\n");
        printf("2. View Route and Timing\n");
        printf("3. Check Seat Availability\n");
        printf("4. Book Seat\n");
        printf("5. Cancel Booking\n");
        printf("6. Student Details\n");
        printf("7. Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                searchBus();
                break;

            case 2:
                viewRouteTiming();
                break;

            case 3:
                checkSeatAvailability();
                break;

            case 4:
                bookSeat();
                break;

            case 5:
                cancelBooking();
                break;

            case 6:
                studentDetails();
                break;

            case 7:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while(choice != 7);
}


/* Student Module */
void studentModule()
{
    initializeSeats();

    if(studentLogin())
    {
        studentMenu();
    }
}