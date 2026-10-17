#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "seat_allocation.h"

#define WAITING_LIST_FILE "data/waitlist.dat"

typedef struct
{
    int busId;
    int studentId;
    char preference[15];
} WaitingEntry;

static int validPreference(const char *preference)
{
    return preference != NULL &&
           (strcmp(preference, "Window") == 0 || strcmp(preference, "Front") == 0 ||
            strcmp(preference, "Any") == 0);
}

static int findBus(int busId, Bus *result)
{
    Bus *buses = NULL;
    void *records = NULL;
    size_t count = 0;
    size_t index;
    int found = 0;

    if (!readRecords(BUSES_FILE, sizeof(Bus), &records, &count))
        return 0;
    buses = records;
    for (index = 0; index < count; index++)
    {
        if (buses[index].busId == busId)
        {
            *result = buses[index];
            found = 1;
            break;
        }
    }
    free(records);
    return found;
}

static int persistSeatsAndCount(Seat *seats, size_t count, Bus oldBus)
{
    size_t index;
    int available = 0;
    Bus updatedBus = oldBus;

    for (index = 0; index < count; index++)
        if (seats[index].busId == oldBus.busId && strcmp(seats[index].status, "Available") == 0)
            available++;
    updatedBus.availableSeats = available;

    if (!updateBus(updatedBus))
        return 0;
    if (!writeRecords(SEATS_FILE, sizeof(Seat), seats, count))
    {
        updateBus(oldBus);
        return 0;
    }
    return 1;
}

static int loadSeats(Seat **seats, size_t *count)
{
    void *records = NULL;
    if (!readRecords(SEATS_FILE, sizeof(Seat), &records, count))
        return 0;
    *seats = records;
    return 1;
}

int getBusSeats(int busId, Seat **seats, size_t *count)
{
    Seat *allSeats = NULL;
    Seat *busSeats = NULL;
    size_t allCount = 0;
    size_t busCount = 0;
    size_t index;

    if (seats == NULL || count == NULL || !initializeBusSeats(busId) ||
        !loadSeats(&allSeats, &allCount))
        return 0;
    for (index = 0; index < allCount; index++)
        if (allSeats[index].busId == busId)
            busCount++;
    if (busCount != 0)
    {
        size_t outputIndex = 0;
        busSeats = malloc(busCount * sizeof(*busSeats));
        if (busSeats == NULL)
        {
            free(allSeats);
            return 0;
        }
        for (index = 0; index < allCount; index++)
            if (allSeats[index].busId == busId)
                busSeats[outputIndex++] = allSeats[index];
    }
    free(allSeats);
    *seats = busSeats;
    *count = busCount;
    return 1;
}

int initializeBusSeats(int busId)
{
    Bus bus;
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;
    int existing = 0;
    Seat *grown;

    if (!findBus(busId, &bus) || bus.capacity < 0 || !loadSeats(&seats, &count))
        return 0;
    for (index = 0; index < count; index++)
        if (seats[index].busId == busId)
            existing++;

    while (existing < bus.capacity)
    {
        Seat seat;
        int seatNumber = 1;
        size_t check;

        for (;; seatNumber++)
        {
            for (check = 0; check < count; check++)
                if (seats[check].busId == busId && seats[check].seatNumber == seatNumber)
                    break;
            if (check == count)
                break;
        }
        grown = realloc(seats, (count + 1) * sizeof(*seats));
        if (grown == NULL)
        {
            free(seats);
            return 0;
        }
        seats = grown;
        memset(&seat, 0, sizeof(seat));
        seat.seatNumber = seatNumber;
        seat.busId = busId;
        snprintf(seat.status, sizeof(seat.status), "Available");
        snprintf(seat.preference, sizeof(seat.preference), "Any");
        seats[count++] = seat;
        existing++;
    }

    if (!persistSeatsAndCount(seats, count, bus))
    {
        free(seats);
        return 0;
    }
    free(seats);
    return 1;
}

int setSeatPreference(int busId, int seatNumber, const char *preference)
{
    Bus bus;
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;
    int found = 0;

    if (seatNumber <= 0 || !validPreference(preference) || !findBus(busId, &bus) ||
        !loadSeats(&seats, &count))
        return 0;
    for (index = 0; index < count; index++)
    {
        if (seats[index].busId == busId && seats[index].seatNumber == seatNumber &&
            strcmp(seats[index].status, "Available") == 0)
        {
            snprintf(seats[index].preference, sizeof(seats[index].preference), "%s", preference);
            found = 1;
            break;
        }
    }
    if (found)
        found = persistSeatsAndCount(seats, count, bus);
    free(seats);
    return found;
}

int displaySeatAvailability(int busId, FILE *output)
{
    Bus bus;
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;

    if (output == NULL)
        output = stdout;
    if (!findBus(busId, &bus) || !initializeBusSeats(busId) || !findBus(busId, &bus) ||
        !loadSeats(&seats, &count))
        return 0;

    fprintf(output, "Bus %d (%s): %d/%d seats available\n", bus.busId, bus.busNumber,
            bus.availableSeats, bus.capacity);
    for (index = 0; index < count; index++)
    {
        if (seats[index].busId == busId)
            fprintf(output, "Seat %d: %s [%s]%s\n", seats[index].seatNumber,
                    seats[index].status, seats[index].preference,
                    strcmp(seats[index].status, "Booked") == 0 ? "" : "");
    }
    free(seats);
    return 1;
}

static int getCandidate(const Seat *seats, size_t count, int busId,
                        const char *preference, size_t *candidate)
{
    size_t index;
    size_t fallback = count;

    for (index = 0; index < count; index++)
    {
        if (seats[index].busId != busId || strcmp(seats[index].status, "Available") != 0)
            continue;
        if (strcmp(preference, "Any") == 0)
        {
            *candidate = index;
            return 1;
        }
        if (strcmp(seats[index].preference, preference) == 0)
        {
            *candidate = index;
            return 1;
        }
        if (strcmp(seats[index].preference, "Any") == 0 && fallback == count)
            fallback = index;
    }
    if (fallback != count)
    {
        *candidate = fallback;
        return 1;
    }
    return 0;
}

static int loadWaiting(WaitingEntry **entries, size_t *count)
{
    void *records = NULL;
    if (!readRecords(WAITING_LIST_FILE, sizeof(WaitingEntry), &records, count))
        return 0;
    *entries = records;
    return 1;
}

static int enqueueWaiting(int busId, int studentId, const char *preference)
{
    WaitingEntry *entries = NULL;
    WaitingEntry *grown;
    size_t count = 0;
    size_t index;

    if (!loadWaiting(&entries, &count))
        return 0;
    for (index = 0; index < count; index++)
        if (entries[index].busId == busId && entries[index].studentId == studentId)
        {
            free(entries);
            return 1;
        }
    grown = realloc(entries, (count + 1) * sizeof(*entries));
    if (grown == NULL)
    {
        free(entries);
        return 0;
    }
    entries = grown;
    entries[count].busId = busId;
    entries[count].studentId = studentId;
    snprintf(entries[count].preference, sizeof(entries[count].preference), "%s", preference);
    count++;
    index = writeRecords(WAITING_LIST_FILE, sizeof(*entries), entries, count);
    free(entries);
    return (int)index;
}

static int removeWaitingAt(WaitingEntry *entries, size_t *count, size_t removed)
{
    if (removed >= *count)
        return 0;
    if (removed + 1 < *count)
        memmove(&entries[removed], &entries[removed + 1], (*count - removed - 1) * sizeof(*entries));
    (*count)--;
    return writeRecords(WAITING_LIST_FILE, sizeof(*entries), entries, *count);
}

static int removeWaitingStudent(int busId, int studentId)
{
    WaitingEntry *entries = NULL;
    size_t count = 0;
    size_t index;
    if (!loadWaiting(&entries, &count))
        return 0;
    for (index = 0; index < count;)
    {
        if (entries[index].busId == busId && entries[index].studentId == studentId)
        {
            if (!removeWaitingAt(entries, &count, index))
            {
                free(entries);
                return 0;
            }
        }
        else
            index++;
    }
    free(entries);
    return 1;
}

static int bookSeat(Seat *seats, size_t count, Bus bus, size_t seatIndex, int studentId)
{
    Seat oldSeat = seats[seatIndex];
    seats[seatIndex].studentId = studentId;
    snprintf(seats[seatIndex].status, sizeof(seats[seatIndex].status), "Booked");
    if (!persistSeatsAndCount(seats, count, bus))
    {
        seats[seatIndex] = oldSeat;
        return 0;
    }
    return 1;
}

static int allocateSeatInternal(int busId, int studentId, int seatNumber,
                                const char *preference, Seat *allocatedSeat)
{
    Bus bus;
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;
    size_t candidate;
    int occupied = 0;

    if (studentId <= 0 || !validPreference(preference) || !findBus(busId, &bus) ||
        !initializeBusSeats(busId) || !loadSeats(&seats, &count))
        return SEAT_ALLOCATION_ERROR;
    if (!findBus(busId, &bus))
    {
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
    for (index = 0; index < count; index++)
    {
        if (seats[index].busId == busId && seats[index].studentId == studentId &&
            strcmp(seats[index].status, "Booked") == 0)
        {
            if (allocatedSeat != NULL)
                *allocatedSeat = seats[index];
            free(seats);
            return SEAT_ALLOCATION_BOOKED;
        }
        if (seats[index].busId == busId && strcmp(seats[index].status, "Booked") == 0)
            occupied++;
    }
    if (occupied >= bus.capacity || bus.availableSeats == 0)
    {
        int queued = enqueueWaiting(busId, studentId, preference);
        free(seats);
        return queued ? SEAT_ALLOCATION_WAITLISTED : SEAT_ALLOCATION_ERROR;
    }
    if (seatNumber == 0)
    {
        if (!getCandidate(seats, count, busId, preference, &candidate))
        {
            free(seats);
            return SEAT_ALLOCATION_ERROR;
        }
    }
    else
    {
        for (candidate = 0; candidate < count; candidate++)
        {
            if (seats[candidate].busId == busId && seats[candidate].seatNumber == seatNumber &&
                strcmp(seats[candidate].status, "Available") == 0 &&
                (strcmp(preference, "Any") == 0 ||
                 strcmp(seats[candidate].preference, preference) == 0 ||
                 strcmp(seats[candidate].preference, "Any") == 0))
                break;
        }
        if (candidate == count)
        {
            free(seats);
            return SEAT_ALLOCATION_ERROR;
        }
    }
    if (!bookSeat(seats, count, bus, candidate, studentId))
    {
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
    if (allocatedSeat != NULL)
        *allocatedSeat = seats[candidate];
    removeWaitingStudent(busId, studentId);
    free(seats);
    return SEAT_ALLOCATION_BOOKED;
}

int allocateSeat(int busId, int studentId, const char *preference, Seat *allocatedSeat)
{
    return allocateSeatInternal(busId, studentId, 0, preference, allocatedSeat);
}

int allocateSpecificSeat(int busId, int studentId, int seatNumber,
                         const char *preference, Seat *allocatedSeat)
{
    if (seatNumber <= 0)
        return SEAT_ALLOCATION_ERROR;
    return allocateSeatInternal(busId, studentId, seatNumber, preference, allocatedSeat);
}

int cancelSeat(int busId, int studentId, Seat *freedSeat, int *promotedStudentId)
{
    Bus bus;
    Seat *seats = NULL;
    WaitingEntry *waiting = NULL;
    size_t count = 0;
    size_t waitingCount = 0;
    size_t index;
    size_t candidate;
    int found = 0;

    if (promotedStudentId != NULL)
        *promotedStudentId = 0;
    if (!findBus(busId, &bus) || !loadSeats(&seats, &count))
        return 0;
    for (index = 0; index < count; index++)
        if (seats[index].busId == busId && seats[index].studentId == studentId &&
            strcmp(seats[index].status, "Booked") == 0)
        {
            found = 1;
            break;
        }
    if (!found)
    {
        free(seats);
        return 0;
    }
    if (freedSeat != NULL)
        *freedSeat = seats[index];
    seats[index].studentId = 0;
    snprintf(seats[index].status, sizeof(seats[index].status), "Available");
    if (!persistSeatsAndCount(seats, count, bus))
    {
        free(seats);
        return 0;
    }

    if (!loadWaiting(&waiting, &waitingCount))
    {
        free(seats);
        return 0;
    }
    for (index = 0; index < waitingCount; index++)
    {
        int promotedId;

        if (waiting[index].busId != busId ||
            !getCandidate(seats, count, busId, waiting[index].preference, &candidate))
            continue;
        promotedId = waiting[index].studentId;
        if (!bookSeat(seats, count, bus, candidate, waiting[index].studentId))
        {
            free(waiting);
            free(seats);
            return 0;
        }
        if (!removeWaitingAt(waiting, &waitingCount, index))
        {
            free(waiting);
            free(seats);
            return 0;
        }
        if (promotedStudentId != NULL)
            *promotedStudentId = promotedId;
        break;
    }
    free(waiting);
    free(seats);
    return 1;
}

int getBusOccupancy(int busId, int *bookedSeats, int *capacity)
{
    Bus bus;
    Seat *seats = NULL;
    size_t count = 0;
    size_t index;
    int booked = 0;

    if (bookedSeats == NULL || capacity == NULL || !findBus(busId, &bus) ||
        !initializeBusSeats(busId) || !loadSeats(&seats, &count))
        return 0;
    for (index = 0; index < count; index++)
        if (seats[index].busId == busId && strcmp(seats[index].status, "Booked") == 0)
            booked++;
    *bookedSeats = booked;
    *capacity = bus.capacity;
    free(seats);
    return 1;
}

int displayBusOccupancy(int busId, FILE *output)
{
    Bus bus;
    Seat *seats = NULL;
    Student *students = NULL;
    void *studentRecords = NULL;
    size_t seatCount = 0;
    size_t studentCount = 0;
    size_t index;
    int booked = 0;

    if (output == NULL)
        output = stdout;
    if (!findBus(busId, &bus) || !initializeBusSeats(busId) ||
        !loadSeats(&seats, &seatCount) ||
        !readRecords(STUDENTS_FILE, sizeof(Student), &studentRecords, &studentCount))
    {
        free(seats);
        return 0;
    }
    students = studentRecords;
    for (index = 0; index < seatCount; index++)
    {
        size_t studentIndex;
        if (seats[index].busId != busId || strcmp(seats[index].status, "Booked") != 0)
            continue;
        booked++;
        fprintf(output, "Seat %d: student %d", seats[index].seatNumber, seats[index].studentId);
        for (studentIndex = 0; studentIndex < studentCount; studentIndex++)
            if (students[studentIndex].studentId == seats[index].studentId)
            {
                fprintf(output, " (%s)", students[studentIndex].name);
                break;
            }
        fputc('\n', output);
    }
    fprintf(output, "Occupancy: %d/%d (%d available)\n", booked, bus.capacity,
            bus.capacity - booked);
    free(studentRecords);
    free(seats);
    return 1;
}

int getWaitingList(int busId, int **studentIds, size_t *count)
{
    WaitingEntry *entries = NULL;
    int *ids = NULL;
    size_t entryCount = 0;
    size_t matches = 0;
    size_t index;

    if (studentIds == NULL || count == NULL || !loadWaiting(&entries, &entryCount))
        return 0;
    *studentIds = NULL;
    *count = 0;
    for (index = 0; index < entryCount; index++)
        if (entries[index].busId == busId)
            matches++;
    if (matches != 0)
    {
        size_t outputIndex = 0;
        ids = malloc(matches * sizeof(*ids));
        if (ids == NULL)
        {
            free(entries);
            return 0;
        }
        for (index = 0; index < entryCount; index++)
            if (entries[index].busId == busId)
                ids[outputIndex++] = entries[index].studentId;
    }
    free(entries);
    *studentIds = ids;
    *count = matches;
    return 1;
}

int displayWaitingList(int busId, FILE *output)
{
    int *studentIds = NULL;
    size_t count = 0;
    size_t index;

    if (output == NULL)
        output = stdout;
    if (!getWaitingList(busId, &studentIds, &count))
        return 0;
    fprintf(output, "Waiting list for bus %d:\n", busId);
    for (index = 0; index < count; index++)
        fprintf(output, "%zu. Student %d\n", index + 1, studentIds[index]);
    free(studentIds);
    return 1;
}