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