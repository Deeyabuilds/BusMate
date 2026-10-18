#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "structures.h"

#define MAX_SCHEDULES 50

extern Schedule schedules[MAX_SCHEDULES];
extern int scheduleCount;

void loadSchedules(void);
int scheduleExists(int scheduleId);
int addSchedule(int scheduleId, int busId, int routeId, const char *startTime, const char *endTime, const char *dayType);
int removeSchedule(int scheduleId);
void displaySchedules(void);

#endif
