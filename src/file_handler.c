#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "file_handler.h"

/* Open a data file */
FILE *openDataFile(const char *path, const char *mode)
{
    return fopen(path, mode);
}

/* Close a data file */
int closeDataFile(FILE *file)
{
    if (file == NULL)
        return 0;

    return fclose(file) == 0;
}

/* Read all records from a file */
int readRecords(const char *path, size_t recordSize,
                void **records, size_t *count)
{
    FILE *file;
    long fileSize;
    size_t recordsCount;
    void *data;

    if (path == NULL || recordSize == 0 ||
        records == NULL || count == NULL)
        return 0;

    *records = NULL;
    *count = 0;

    file = fopen(path, "rb");

    if (file == NULL)
        return 1;

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return 0;
    }

    fileSize = ftell(file);

    if (fileSize < 0 || (size_t)fileSize % recordSize != 0)
    {
        fclose(file);
        return 0;
    }

    recordsCount = (size_t)fileSize / recordSize;

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        fclose(file);
        return 0;
    }

    if (recordsCount == 0)
    {
        fclose(file);
        return 1;
    }

    data = malloc(recordsCount * recordSize);

    if (data == NULL)
    {
        fclose(file);
        return 0;
    }

    if (fread(data, recordSize, recordsCount, file) != recordsCount)
    {
        free(data);
        fclose(file);
        return 0;
    }

    fclose(file);

    *records = data;
    *count = recordsCount;

    return 1;
}

/* Write records to a file */
int writeRecords(const char *path, size_t recordSize,
                 const void *records, size_t count)
{
    FILE *file;

    if (path == NULL || recordSize == 0 ||
        (count > 0 && records == NULL))
        return 0;

    file = fopen(path, "wb");

    if (file == NULL)
        return 0;

    if (count > 0 &&
        fwrite(records, recordSize, count, file) != count)
    {
        fclose(file);
        return 0;
    }

    return fclose(file) == 0;
}

/* Append one record */
int appendRecord(const char *path, size_t recordSize,
                 const void *record)
{
    FILE *file;
    int success;

    if (path == NULL || recordSize == 0 || record == NULL)
        return 0;

    file = fopen(path, "ab");

    if (file == NULL)
        return 0;

    success = fwrite(record, recordSize, 1, file) == 1;

    if (fclose(file) != 0)
        success = 0;

    return success;
}

/* Update a record using its ID */
int updateRecordById(const char *path, size_t recordSize,
                     size_t idOffset, int id,
                     const void *replacement, int *updated)
{
    void *records = NULL;
    size_t count = 0;
    size_t i;
    int success = 0;

    if (updated != NULL)
        *updated = 0;

    if (path == NULL || recordSize == 0 ||
        replacement == NULL || updated == NULL ||
        idOffset > recordSize ||
        sizeof(int) > recordSize - idOffset)
        return 0;

    if (!readRecords(path, recordSize, &records, &count))
        return 0;

    for (i = 0; i < count; i++)
    {
        unsigned char *record =
            (unsigned char *)records + i * recordSize;

        int recordId;
        memcpy(&recordId, record + idOffset, sizeof(int));

        if (recordId == id)
        {
            memcpy(record, replacement, recordSize);
            *updated = 1;
            break;
        }
    }

    success = writeRecords(path, recordSize, records, count);

    free(records);

    return success;
}

/* Delete a record using its ID */
int deleteRecordById(const char *path, size_t recordSize,
                     size_t idOffset, int id, int *deleted)
{
    void *records = NULL;
    void *filtered = NULL;
    size_t count = 0;
    size_t kept = 0;
    size_t i;
    int success;

    if (deleted != NULL)
        *deleted = 0;

    if (path == NULL || recordSize == 0 ||
        deleted == NULL ||
        idOffset > recordSize ||
        sizeof(int) > recordSize - idOffset)
        return 0;

    if (!readRecords(path, recordSize, &records, &count))
        return 0;

    if (count > 0)
    {
        filtered = malloc(count * recordSize);

        if (filtered == NULL)
        {
            free(records);
            return 0;
        }
    }

    for (i = 0; i < count; i++)
    {
        unsigned char *record =
            (unsigned char *)records + i * recordSize;

        int recordId;
        memcpy(&recordId, record + idOffset, sizeof(int));

        if (recordId == id)
        {
            *deleted = 1;
        }
        else
        {
            memcpy((unsigned char *)filtered + kept * recordSize,
                   record, recordSize);
            kept++;
        }
    }

    success = writeRecords(path, recordSize, filtered, kept);

    free(records);
    free(filtered);

    return success;
}

/* Save functions used by the Admin module and other modules */
void saveStudent(Student student)
{
    appendRecord(STUDENTS_FILE, sizeof(Student), &student);
}

void saveBus(Bus bus)
{
    appendRecord(BUSES_FILE, sizeof(Bus), &bus);
}

void saveRoute(Route route)
{
    appendRecord(ROUTES_FILE, sizeof(Route), &route);
}

void saveDriver(Driver driver)
{
    appendRecord(DRIVERS_FILE, sizeof(Driver), &driver);
}

void saveSchedule(Schedule schedule)
{
    appendRecord(SCHEDULES_FILE, sizeof(Schedule), &schedule);
}

int saveSeat(Seat seat)
{
    return appendRecord(SEATS_FILE, sizeof(Seat), &seat);
}

/* Update existing records */
int updateStudent(Student student)
{
    int updated;

    if (!updateRecordById(STUDENTS_FILE, sizeof(Student),
                          offsetof(Student, studentId),
                          student.studentId, &student, &updated))
        return 0;

    return updated;
}

int updateBus(Bus bus)
{
    int updated;

    if (!updateRecordById(BUSES_FILE, sizeof(Bus),
                          offsetof(Bus, busId),
                          bus.busId, &bus, &updated))
        return 0;

    return updated;
}

int updateRoute(Route route)
{
    int updated;

    if (!updateRecordById(ROUTES_FILE, sizeof(Route),
                          offsetof(Route, routeId),
                          route.routeId, &route, &updated))
        return 0;

    return updated;
}

int updateDriver(Driver driver)
{
    int updated;

    if (!updateRecordById(DRIVERS_FILE, sizeof(Driver),
                          offsetof(Driver, driverId),
                          driver.driverId, &driver, &updated))
        return 0;

    return updated;
}

int updateSchedule(Schedule schedule)
{
    int updated;

    if (!updateRecordById(SCHEDULES_FILE, sizeof(Schedule),
                          offsetof(Schedule, scheduleId),
                          schedule.scheduleId, &schedule, &updated))
        return 0;

    return updated;
}

/* Update seat status and student assignment */
int updateSeatStatus(int busId, int seatNumber,
                     const char *status, int studentId)
{
    FILE *file;
    Seat seat;
    int found = 0;

    if (status == NULL)
        return 0;

    file = fopen(SEATS_FILE, "rb+");

    if (file == NULL)
        return 0;

    while (fread(&seat, sizeof(Seat), 1, file) == 1)
    {
        if (seat.busId == busId &&
            seat.seatNumber == seatNumber)
        {
            strncpy(seat.status, status, sizeof(seat.status) - 1);
            seat.status[sizeof(seat.status) - 1] = '\0';
            seat.studentId = studentId;

            if (fseek(file, -(long)sizeof(Seat), SEEK_CUR) != 0 ||
                fwrite(&seat, sizeof(Seat), 1, file) != 1)
            {
                fclose(file);
                return 0;
            }

            found = 1;
            break;
        }
    }

    fclose(file);

    return found;
}