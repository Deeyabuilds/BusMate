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

static int studentExists(int studentId)
{
    Student *students = NULL;
    void *records = NULL;
    size_t count = 0;
    size_t index;
    int found = 0;

    if (studentId <= 0 || !readRecords(STUDENTS_FILE, sizeof(Student), &records, &count))
        return 0;
    students = records;
    for (index = 0; index < count; index++)
        if (students[index].studentId == studentId)
        {
            found = 1;
            break;
        }
    free(records);
    return found;
}

static int validateBusSeats(const Seat *seats, size_t count, int busId, int capacity)
{
    size_t index;
    size_t previous;

    for (index = 0; index < count; index++)
    {
        if (seats[index].busId != busId)
            continue;
        if (seats[index].seatNumber <= 0 || seats[index].seatNumber > capacity ||
            (strcmp(seats[index].status, "Available") != 0 &&
             strcmp(seats[index].status, "Booked") != 0) ||
            !validPreference(seats[index].preference) ||
            (strcmp(seats[index].status, "Booked") == 0 && seats[index].studentId <= 0) ||
            (strcmp(seats[index].status, "Available") == 0 && seats[index].studentId != 0))
            return 0;
        for (previous = 0; previous < index; previous++)
            if (seats[previous].busId == busId &&
                (seats[previous].seatNumber == seats[index].seatNumber ||
                 (strcmp(seats[index].status, "Booked") == 0 &&
                  strcmp(seats[previous].status, "Booked") == 0 &&
                  seats[previous].studentId == seats[index].studentId)))
                return 0;
    }
    return 1;
}

static size_t countStudentBookings(const Seat *seats, size_t count,
                                   int studentId, size_t *firstBooking)
{
    size_t index;
    size_t matches = 0;

    for (index = 0; index < count; index++)
        if (seats[index].studentId == studentId &&
            strcmp(seats[index].status, "Booked") == 0)
        {
            if (matches == 0 && firstBooking != NULL)
                *firstBooking = index;
            matches++;
        }
    return matches;
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
    size_t busSeatCount = 0;
    int available = 0;
    Bus updatedBus = oldBus;

    if (oldBus.capacity <= 0 ||
        !validateBusSeats(seats, count, oldBus.busId, oldBus.capacity))
        return 0;
    for (index = 0; index < count; index++)
        if (seats[index].busId == oldBus.busId)
        {
            busSeatCount++;
            if (strcmp(seats[index].status, "Available") == 0)
                available++;
        }
    if (busSeatCount != (size_t)oldBus.capacity)
        return 0;
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

int validateBusSeatData(int busId)
{
    Bus *buses = NULL;
    Seat *seats = NULL;
    void *busRecords = NULL;
    size_t busCount = 0;
    size_t seatCount = 0;
    size_t index;
    size_t matchingBuses = 0;
    size_t matchingSeats = 0;
    int available = 0;
    int capacity = 0;
    int recordedAvailable = 0;
    Bus *bus = NULL;

    if (busId <= 0 || !readRecords(BUSES_FILE, sizeof(Bus), &busRecords, &busCount) ||
        !loadSeats(&seats, &seatCount))
    {
        free(busRecords);
        free(seats);
        return 0;
    }
    buses = busRecords;
    for (index = 0; index < busCount; index++)
        if (buses[index].busId == busId)
        {
            matchingBuses++;
            bus = &buses[index];
        }
    if (matchingBuses != 1 || bus == NULL || bus->capacity <= 0 ||
        bus->availableSeats < 0 || bus->availableSeats > bus->capacity ||
        !validateBusSeats(seats, seatCount, busId, bus->capacity))
    {
        free(busRecords);
        free(seats);
        return 0;
    }
    for (index = 0; index < seatCount; index++)
        if (seats[index].busId == busId)
        {
            matchingSeats++;
            if (strcmp(seats[index].status, "Available") == 0)
                available++;
        }
    capacity = bus->capacity;
    recordedAvailable = bus->availableSeats;
    free(busRecords);
    free(seats);
    return matchingSeats == (size_t)capacity && available == recordedAvailable;
}

int createBusWithSeats(const Bus *bus)
{
    void *records = NULL;
    Seat *seats = NULL;
    size_t busCount = 0;
    size_t seatCount = 0;
    size_t index;
    int deleted = 0;
    Bus newBus;

    if (bus == NULL || bus->busId <= 0 || bus->capacity <= 0 ||
        memchr(bus->busNumber, '\0', sizeof(bus->busNumber)) == NULL ||
        bus->busNumber[0] == '\0' ||
        !readRecords(BUSES_FILE, sizeof(Bus), &records, &busCount))
        return 0;
    for (index = 0; index < busCount; index++)
        if (((Bus *)records)[index].busId == bus->busId)
        {
            free(records);
            return 0;
        }
    free(records);
    if (!loadSeats(&seats, &seatCount))
        return 0;
    for (index = 0; index < seatCount; index++)
        if (seats[index].busId == bus->busId)
        {
            free(seats);
            return 0;
        }
    free(seats);

    newBus = *bus;
    newBus.availableSeats = newBus.capacity;
    if (!appendRecord(BUSES_FILE, sizeof(newBus), &newBus))
        return 0;
    if (initializeBusSeats(newBus.busId) && validateBusSeatData(newBus.busId))
        return 1;

    deleteRecordById(SEATS_FILE, sizeof(Seat), offsetof(Seat, busId),
                     newBus.busId, &deleted);
    deleteRecordById(BUSES_FILE, sizeof(Bus), offsetof(Bus, busId),
                     newBus.busId, &deleted);
    return 0;
}

int removeBusWithSeats(int busId)
{
    Bus *buses;
    Seat *seats = NULL;
    void *busRecords = NULL;
    void *waitingRecords = NULL;
    size_t busCount = 0;
    size_t seatCount = 0;
    size_t waitingCount = 0;
    size_t index;
    size_t matchingBuses = 0;
    size_t keptBuses = 0;
    size_t keptSeats = 0;
    size_t seatOutputIndex = 0;
    Bus *updatedBuses;
    Seat *updatedSeats;
    int busFound = 0;

    if (busId <= 0 || !validateBusSeatData(busId) ||
        !readRecords(BUSES_FILE, sizeof(Bus), &busRecords, &busCount) ||
        !loadSeats(&seats, &seatCount) ||
        !readRecords(WAITING_LIST_FILE, sizeof(WaitingEntry),
                     &waitingRecords, &waitingCount))
    {
        free(busRecords);
        free(seats);
        free(waitingRecords);
        return 0;
    }
    buses = busRecords;
    for (index = 0; index < busCount; index++)
        if (buses[index].busId == busId)
        {
            matchingBuses++;
            busFound = 1;
            if (buses[index].availableSeats != buses[index].capacity)
                goto refuseRemoval;
        }
    if (!busFound || matchingBuses != 1)
        goto refuseRemoval;
    for (index = 0; index < seatCount; index++)
        if (seats[index].busId != busId)
            keptSeats++;
    for (index = 0; index < waitingCount; index++)
        if (((WaitingEntry *)waitingRecords)[index].busId == busId)
            goto refuseRemoval;

    updatedBuses = malloc(busCount * sizeof(*updatedBuses));
    updatedSeats = keptSeats == 0 ? NULL : malloc(keptSeats * sizeof(*updatedSeats));
    if (updatedBuses == NULL || (keptSeats != 0 && updatedSeats == NULL))
    {
        free(updatedBuses);
        free(updatedSeats);
        goto refuseRemoval;
    }
    for (index = 0; index < busCount; index++)
        if (buses[index].busId != busId)
            updatedBuses[keptBuses++] = buses[index];
    keptBuses = 0;
    for (index = 0; index < seatCount; index++)
        if (seats[index].busId != busId)
            updatedSeats[seatOutputIndex++] = seats[index];

    if (!writeRecords(SEATS_FILE, sizeof(Seat), updatedSeats, keptSeats))
    {
        free(updatedBuses);
        free(updatedSeats);
        goto refuseRemoval;
    }
    if (!writeRecords(BUSES_FILE, sizeof(Bus), updatedBuses, busCount - 1))
    {
        writeRecords(SEATS_FILE, sizeof(Seat), seats, seatCount);
        free(updatedBuses);
        free(updatedSeats);
        goto refuseRemoval;
    }
    free(updatedBuses);
    free(updatedSeats);
    free(busRecords);
    free(seats);
    free(waitingRecords);
    return 1;

refuseRemoval:
    free(busRecords);
    free(seats);
    free(waitingRecords);
    return 0;
}

int getBusSeats(int busId, Seat **seats, size_t *count)
{
    Seat *allSeats = NULL;
    Seat *busSeats = NULL;
    size_t allCount = 0;
    size_t busCount = 0;
    size_t index;

    if (seats == NULL || count == NULL)
        return 0;
    *seats = NULL;
    *count = 0;
    if (!initializeBusSeats(busId) ||
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
    size_t existing = 0;
    Seat *grown;

    if (!findBus(busId, &bus) || bus.capacity <= 0 || !loadSeats(&seats, &count))
        return 0;
    if (!validateBusSeats(seats, count, busId, bus.capacity))
    {
        free(seats);
        return 0;
    }
    for (index = 0; index < count; index++)
        if (seats[index].busId == busId)
            existing++;

    while (existing < (size_t)bus.capacity)
    {
        Seat seat;
        int seatNumber;
        size_t check;

        for (seatNumber = 1; seatNumber <= bus.capacity; seatNumber++)
        {
            for (check = 0; check < count; check++)
                if (seats[check].busId == busId && seats[check].seatNumber == seatNumber)
                    break;
            if (check == count)
                break;
        }
        if (seatNumber > bus.capacity || count >= (size_t)-1 / sizeof(*seats))
        {
            free(seats);
            return 0;
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
        !initializeBusSeats(busId) ||
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

static int removeWaitingStudent(int studentId)
{
    WaitingEntry *entries = NULL;
    size_t count = 0;
    size_t index;
    if (!loadWaiting(&entries, &count))
        return 0;
    for (index = 0; index < count;)
    {
        if (entries[index].studentId == studentId)
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
    size_t firstBooking = 0;
    size_t studentBookingCount;
    int occupied = 0;

    if (!studentExists(studentId) || !validPreference(preference) || !findBus(busId, &bus) ||
        !initializeBusSeats(busId) || !loadSeats(&seats, &count))
        return SEAT_ALLOCATION_ERROR;
    if (!findBus(busId, &bus))
    {
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
    studentBookingCount = countStudentBookings(seats, count, studentId, &firstBooking);
    if (studentBookingCount > 1)
    {
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
    if (studentBookingCount == 1)
    {
        int existingBusMatches = seats[firstBooking].busId == busId;
        if (existingBusMatches && allocatedSeat != NULL)
            *allocatedSeat = seats[firstBooking];
        free(seats);
        return existingBusMatches ? SEAT_ALLOCATION_BOOKED : SEAT_ALLOCATION_ERROR;
    }
    for (index = 0; index < count; index++)
        if (seats[index].busId == busId && strcmp(seats[index].status, "Booked") == 0)
            occupied++;
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
        size_t seatMatches = 0;
        size_t matchingCandidate = count;

        for (index = 0; index < count; index++)
        {
            if (seats[index].busId != busId || seats[index].seatNumber != seatNumber)
                continue;
            seatMatches++;
            if (strcmp(seats[index].status, "Available") == 0 &&
                (strcmp(preference, "Any") == 0 ||
                 strcmp(seats[index].preference, preference) == 0 ||
                 strcmp(seats[index].preference, "Any") == 0))
                matchingCandidate = index;
        }
        if (seatMatches != 1 || matchingCandidate == count)
        {
            free(seats);
            return SEAT_ALLOCATION_ERROR;
        }
        candidate = matchingCandidate;
    }
    if (!bookSeat(seats, count, bus, candidate, studentId))
    {
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
    if (allocatedSeat != NULL)
        *allocatedSeat = seats[candidate];
    if (!removeWaitingStudent(studentId))
    {
        seats[candidate].studentId = 0;
        snprintf(seats[candidate].status, sizeof(seats[candidate].status), "Available");
        persistSeatsAndCount(seats, count, bus);
        free(seats);
        return SEAT_ALLOCATION_ERROR;
    }
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

SeatCancellationResult cancelSeat(int busId, int studentId, Seat *freedSeat,
                                  int *promotedStudentId)
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
    if (studentId <= 0 || !studentExists(studentId) || !findBus(busId, &bus) ||
        !initializeBusSeats(busId) || !findBus(busId, &bus) || !loadSeats(&seats, &count))
        return SEAT_CANCELLATION_ERROR;
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
        return SEAT_CANCELLATION_NOT_FOUND;
    }
    if (freedSeat != NULL)
        *freedSeat = seats[index];
    seats[index].studentId = 0;
    snprintf(seats[index].status, sizeof(seats[index].status), "Available");
    if (!persistSeatsAndCount(seats, count, bus))
    {
        free(seats);
        return SEAT_CANCELLATION_ERROR;
    }

    if (!loadWaiting(&waiting, &waitingCount))
    {
        free(seats);
        return SEAT_CANCELLATION_PROMOTION_FAILED;
    }
    for (index = 0; index < waitingCount;)
    {
        int queuedStudentId = waiting[index].studentId;
        size_t existingBookings;

        if (waiting[index].busId != busId ||
            queuedStudentId <= 0)
        {
            index++;
            continue;
        }
        if (!studentExists(queuedStudentId))
        {
            if (!removeWaitingStudent(queuedStudentId) ||
                !loadWaiting(&waiting, &waitingCount))
            {
                free(waiting);
                free(seats);
                return SEAT_CANCELLATION_PROMOTION_FAILED;
            }
            index = 0;
            continue;
        }
        existingBookings = countStudentBookings(seats, count, queuedStudentId, NULL);
        if (existingBookings != 0)
        {
            if (!removeWaitingStudent(queuedStudentId) ||
                !loadWaiting(&waiting, &waitingCount))
            {
                free(waiting);
                free(seats);
                return SEAT_CANCELLATION_PROMOTION_FAILED;
            }
            index = 0;
            continue;
        }
        if (!getCandidate(seats, count, busId, waiting[index].preference, &candidate))
        {
            index++;
            continue;
        }
        if (!bookSeat(seats, count, bus, candidate, queuedStudentId))
        {
            free(waiting);
            free(seats);
            return SEAT_CANCELLATION_PROMOTION_FAILED;
        }
        if (!removeWaitingStudent(queuedStudentId))
        {
            seats[candidate].studentId = 0;
            snprintf(seats[candidate].status, sizeof(seats[candidate].status), "Available");
            persistSeatsAndCount(seats, count, bus);
            free(waiting);
            free(seats);
            return SEAT_CANCELLATION_PROMOTION_FAILED;
        }
        if (promotedStudentId != NULL)
            *promotedStudentId = queuedStudentId;
        free(waiting);
        free(seats);
        return SEAT_CANCELLATION_PROMOTED;
    }
    free(waiting);
    free(seats);
    return SEAT_CANCELLATION_COMPLETED;
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