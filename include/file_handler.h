#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "structures.h"

/* Student */
void saveStudent(Student student);
void viewStudents(void);
void updateStudent(Student student);
void deleteStudent(char student_id[]);

/* Bus */
void saveBus(Bus bus);
void viewBuses(void);
void updateBus(Bus bus);
void deleteBus(char bus_id[]);

/* Route */
void saveRoute(Route route);
void viewRoutes(void);
void updateRoute(Route route);
void deleteRoute(char route_id[]);

/* Driver */
void saveDriver(Driver driver);
void viewDrivers(void);
void updateDriver(Driver driver);
void deleteDriver(char driver_id[]);

/* Schedule */
void saveSchedule(Schedule schedule);
void viewSchedules(void);
void updateSchedule(Schedule schedule);
void deleteSchedule(char schedule_id[]);

/* Booking */
void saveBooking(Booking booking);
void viewBookings(void);
void deleteBooking(char student_id[], char schedule_id[]);

#endif