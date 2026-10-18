#include <stdio.h>
#include <string.h>
#include "schedule.h"
#include "file_handler.h"

Schedule schedules[MAX_SCHEDULES];
int scheduleCount = 0;

void loadSchedules(void)
{
    scheduleCount = 0;
    FILE *file = fopen("data/schedules.dat", "rb");
    if (file == NULL)
        return;

    while (scheduleCount < MAX_SCHEDULES && fread(&schedules[scheduleCount], sizeof(Schedule), 1, file) == 1)
    {
        scheduleCount++;
    }

    fclose(file);
}

int scheduleExists(int scheduleId)
{
    loadSchedules();
    for (int i = 0; i < scheduleCount; i++)
    {
        if (schedules[i].scheduleId == scheduleId)
            return 1;
    }
    return 0;
}

int addSchedule(int scheduleId, int busId, int routeId, const char *startTime, const char *endTime, const char *dayType)
{
    loadSchedules();

    if (scheduleCount >= MAX_SCHEDULES)
    {
        printf("Schedule limit reached!\n");
        return -1;
    }

    if (scheduleExists(scheduleId))
    {
        printf("Schedule ID already exists!\n");
        return -2;
    }

    Schedule newSched;
    newSched.scheduleId = scheduleId;
    newSched.busId = busId;
    newSched.routeId = routeId;
    snprintf(newSched.startTime, sizeof(newSched.startTime), "%s", startTime ? startTime : "");
    snprintf(newSched.endTime, sizeof(newSched.endTime), "%s", endTime ? endTime : "");
    snprintf(newSched.dayType, sizeof(newSched.dayType), "%s", dayType ? dayType : "");

    saveSchedule(newSched);
    loadSchedules();

    printf("Schedule added successfully!\n");
    return 0;
}

void displaySchedules(void)
{
    loadSchedules();

    if (scheduleCount == 0)
    {
        printf("\nNo schedules available.\n");
        return;
    }

    printf("\n------ BUS SCHEDULES ------\n");

    for (int i = 0; i < scheduleCount; i++)
    {
        printf("\nSchedule ID: %d", schedules[i].scheduleId);
        printf("\nBus ID      : %d", schedules[i].busId);
        printf("\nRoute ID    : %d", schedules[i].routeId);
        printf("\nStart Time  : %s", schedules[i].startTime);
        printf("\nEnd Time    : %s", schedules[i].endTime);
        printf("\nDay Type    : %s\n", schedules[i].dayType);
    }
}

int removeSchedule(int scheduleId)
{
    loadSchedules();

    FILE *file = fopen("data/schedules.dat", "rb");
    if (file == NULL)
    {
        printf("Unable to open schedule records.\n");
        return -1;
    }

    FILE *temp = fopen("data/temp_sched.dat", "wb");
    if (temp == NULL)
    {
        fclose(file);
        printf("Unable to write temporary file.\n");
        return -1;
    }

    Schedule schedule;
    int found = 0;

    while (fread(&schedule, sizeof(Schedule), 1, file) == 1)
    {
        if (schedule.scheduleId == scheduleId)
        {
            found = 1;
            continue;
        }

        fwrite(&schedule, sizeof(Schedule), 1, temp);
    }

    fclose(file);
    fclose(temp);

    remove("data/schedules.dat");
    rename("data/temp_sched.dat", "data/schedules.dat");

    loadSchedules();

    if (found)
    {
        printf("Schedule removed successfully!\n");
        return 0;
    }
    else
    {
        printf("Schedule ID not found!\n");
        return -1;
    }
}