#include <stdio.h>
#include "file_handler.h"

void saveStudent(Student student)
{
    FILE *file = fopen("data/students.dat", "ab");

    if (file == NULL)
        return;

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
}

void saveBus(Bus bus)
{
    FILE *file = fopen("data/buses.dat", "ab");

    if (file == NULL)
        return;

    fwrite(&bus, sizeof(Bus), 1, file);
    fclose(file);
}

void saveRoute(Route route)
{
    FILE *file = fopen("data/routes.dat", "ab");

    if (file == NULL)
        return;

    fwrite(&route, sizeof(Route), 1, file);
    fclose(file);
}

void saveDriver(Driver driver)
{
    FILE *file = fopen("data/drivers.dat", "ab");

    if (file == NULL)
        return;

    fwrite(&driver, sizeof(Driver), 1, file);
    fclose(file);
}

void saveSchedule(Schedule schedule)
{
    FILE *file = fopen("data/schedules.dat", "ab");

    if (file == NULL)
        return;

    fwrite(&schedule, sizeof(Schedule), 1, file);
    fclose(file);
}

// Seat Allocation & Waiting List Storage

bool init_storage_system(void)
{
    FILE *fp1 = fopen(SEAT_DATA_FILE, "ab+");

    if (!fp1)
        return false;

    fclose(fp1);

    FILE *fp2 = fopen(WAITLIST_FILE, "ab+");

    if (!fp2)
        return false;

    fclose(fp2);

    return true;
}

bool save_waiting_student(const Student *student)
{
    if (!student)
        return false;

    FILE *fp = fopen(WAITLIST_FILE, "ab");

    if (!fp)
        return false;

    size_t written = fwrite(student, sizeof(Student), 1, fp);

    fclose(fp);

    return written == 1;
}