#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "file_handler.h"

FILE *openDataFile(const char *path, const char *mode)
{
    if (path == NULL || mode == NULL)
        return NULL;
    return fopen(path, mode);
}

int closeDataFile(FILE *file)
{
    return file != NULL && fclose(file) == 0;
}

int readRecords(const char *path, size_t recordSize, void **records, size_t *count)
{
    FILE *file;
    unsigned char *data = NULL;
    size_t used = 0;
    size_t capacity = 0;

    if (path == NULL || recordSize == 0 || records == NULL || count == NULL)
        return 0;

    *records = NULL;
    *count = 0;
    file = openDataFile(path, "rb");
    if (file == NULL)
        return errno == ENOENT;

    for (;;)
    {
        size_t readCount;

        if (used == capacity)
        {
            size_t nextCapacity = capacity == 0 ? 16 : capacity * 2;
            unsigned char *grown;

            if (nextCapacity < capacity || nextCapacity > (size_t)-1 / recordSize)
            {
                free(data);
                fclose(file);
                return 0;
            }
            grown = realloc(data, nextCapacity * recordSize);
            if (grown == NULL)
            {
                free(data);
                fclose(file);
                return 0;
            }
            data = grown;
            capacity = nextCapacity;
        }

        readCount = fread(data + used * recordSize, 1, recordSize, file);
        if (readCount == recordSize)
        {
            used++;
            continue;
        }
        if (ferror(file))
        {
            free(data);
            fclose(file);
            return 0;
        }
        if (readCount != 0)
        {
            free(data);
            fclose(file);
            return 0;
        }
        break;
    }

    if (!closeDataFile(file))
    {
        free(data);
        return 0;
    }

    *records = data;
    *count = used;
    return 1;
}

int writeRecords(const char *path, size_t recordSize, const void *records, size_t count)
{
    const char *temporaryPath = "data/temp.dat";
    const char *backupPath = "data/temp.bak";
    FILE *file;
    int hadOriginal = 0;

    if (path == NULL || recordSize == 0 || (count != 0 && records == NULL) ||
        count > (size_t)-1 / recordSize || strcmp(path, temporaryPath) == 0)
        return 0;

    file = openDataFile(temporaryPath, "wb");
    if (file == NULL)
        return 0;
    if (count != 0 && fwrite(records, recordSize, count, file) != count)
    {
        fclose(file);
        remove(temporaryPath);
        return 0;
    }
    if (fclose(file) != 0)
    {
        remove(temporaryPath);
        return 0;
    }

    errno = 0;
    if (rename(path, backupPath) == 0)
        hadOriginal = 1;
    else if (errno != ENOENT)
    {
        remove(temporaryPath);
        return 0;
    }

    if (rename(temporaryPath, path) != 0)
    {
        if (hadOriginal)
            rename(backupPath, path);
        remove(temporaryPath);
        return 0;
    }
    if (hadOriginal)
        remove(backupPath);
    return 1;
}

int appendRecord(const char *path, size_t recordSize, const void *record)
{
    FILE *file;
    int success;

    if (path == NULL || recordSize == 0 || record == NULL)
        return 0;
    file = openDataFile(path, "ab");
    if (file == NULL)
        return 0;
    success = fwrite(record, recordSize, 1, file) == 1;
    if (fclose(file) != 0)
        success = 0;
    return success;
}

static int changeRecordById(const char *path, size_t recordSize, size_t idOffset,
                            int id, const void *replacement, int removeMatches,
                            int *changed)
{
    void *records = NULL;
    size_t count = 0;
    size_t kept = 0;
    size_t index;
    int found = 0;
    unsigned char *bytes;

    if (changed != NULL)
        *changed = 0;
    if (path == NULL || recordSize < sizeof(int) ||
        idOffset > recordSize - sizeof(int) ||
        (!removeMatches && replacement == NULL) ||
        !readRecords(path, recordSize, &records, &count))
        return 0;

    if (!removeMatches)
    {
        int replacementId;
        memcpy(&replacementId, (const unsigned char *)replacement + idOffset,
               sizeof(replacementId));
        if (replacementId != id)
        {
            free(records);
            return 0;
        }
    }

    bytes = records;
    for (index = 0; index < count; index++)
    {
        unsigned char *record = bytes + index * recordSize;
        int recordId;

        memcpy(&recordId, record + idOffset, sizeof(recordId));
        if (recordId == id)
        {
            found = 1;
            if (removeMatches)
                continue;
            memcpy(record, replacement, recordSize);
        }
        if (kept != index)
            memmove(bytes + kept * recordSize, record, recordSize);
        kept++;
    }

    if (found && !writeRecords(path, recordSize, records, removeMatches ? kept : count))
    {
        free(records);
        return 0;
    }
    free(records);
    if (changed != NULL)
        *changed = found;
    return 1;
}

int updateRecordById(const char *path, size_t recordSize, size_t idOffset,
                     int id, const void *replacement, int *updated)
{
    return changeRecordById(path, recordSize, idOffset, id, replacement, 0, updated);
}

int deleteRecordById(const char *path, size_t recordSize, size_t idOffset,
                     int id, int *deleted)
{
    return changeRecordById(path, recordSize, idOffset, id, NULL, 1, deleted);
}

static void saveRecord(const char *path, size_t recordSize, const void *record)
{
    if (!appendRecord(path, recordSize, record))
        fprintf(stderr, "Unable to save record to %s.\n", path);
}

void saveStudent(Student student)
{
    saveRecord(STUDENTS_FILE, sizeof(student), &student);
}

void saveBus(Bus bus)
{
    saveRecord(BUSES_FILE, sizeof(bus), &bus);
}

void saveRoute(Route route)
{
    saveRecord(ROUTES_FILE, sizeof(route), &route);
}

void saveDriver(Driver driver)
{
    saveRecord(DRIVERS_FILE, sizeof(driver), &driver);
}

void saveSchedule(Schedule schedule)
{
    saveRecord(SCHEDULES_FILE, sizeof(schedule), &schedule);
}

int saveSeat(Seat seat)
{
    return appendRecord(SEATS_FILE, sizeof(seat), &seat);
}

int updateStudent(Student student)
{
    int updated = 0;
    return updateRecordById(STUDENTS_FILE, sizeof(student), offsetof(Student, studentId),
                            student.studentId, &student, &updated) &&
           updated;
}

int updateBus(Bus bus)
{
    int updated = 0;
    return updateRecordById(BUSES_FILE, sizeof(bus), offsetof(Bus, busId), bus.busId,
                            &bus, &updated) &&
           updated;
}

int updateRoute(Route route)
{
    int updated = 0;
    return updateRecordById(ROUTES_FILE, sizeof(route), offsetof(Route, routeId), route.routeId,
                            &route, &updated) &&
           updated;
}

int updateDriver(Driver driver)
{
    int updated = 0;
    return updateRecordById(DRIVERS_FILE, sizeof(driver), offsetof(Driver, driverId),
                            driver.driverId, &driver, &updated) &&
           updated;
}

int updateSchedule(Schedule schedule)
{
    int updated = 0;
    return updateRecordById(SCHEDULES_FILE, sizeof(schedule), offsetof(Schedule, scheduleId),
                            schedule.scheduleId, &schedule, &updated) &&
           updated;
}

int updateSeatStatus(int busId, int seatNumber, const char *status, int studentId)
{
    Seat *seats = NULL;
    void *records = NULL;
    size_t count = 0;
    size_t index;
    int found = 0;

    if (status == NULL || strlen(status) >= sizeof(((Seat *)0)->status) ||
        !readRecords(SEATS_FILE, sizeof(Seat), &records, &count))
        return 0;
    seats = records;
    for (index = 0; index < count; index++)
    {
        if (seats[index].busId == busId && seats[index].seatNumber == seatNumber)
        {
            snprintf(seats[index].status, sizeof(seats[index].status), "%s", status);
            seats[index].studentId = studentId;
            found = 1;
            break;
        }
    }
    if (found)
        found = writeRecords(SEATS_FILE, sizeof(Seat), seats, count);
    free(records);
    return found;
}
