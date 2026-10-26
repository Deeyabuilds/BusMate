#ifndef STRUCTURES_H
#define STRUCTURES_H

#define ID_LENGTH 20
#define NAME_LENGTH 50
#define PASSWORD_LENGTH 50
#define LOCATION_LENGTH 50
#define TIME_LENGTH 20

typedef struct {
    char student_id[ID_LENGTH];
    char name[NAME_LENGTH];
    char password[PASSWORD_LENGTH];
} Student;

typedef struct {
    char admin_id[ID_LENGTH];
    char username[NAME_LENGTH];
    char password[PASSWORD_LENGTH];
} Admin;

typedef struct {
    char bus_id[ID_LENGTH];
    char bus_number[ID_LENGTH];
    int capacity;
} Bus;

typedef struct {
    char route_id[ID_LENGTH];
    char starting_point[LOCATION_LENGTH];
    char destination[LOCATION_LENGTH];
} Route;

typedef struct {
    char driver_id[ID_LENGTH];
    char name[NAME_LENGTH];
    char assigned_bus_id[ID_LENGTH];
} Driver;

typedef struct {
    char schedule_id[ID_LENGTH];
    char bus_id[ID_LENGTH];
    char route_id[ID_LENGTH];
    char departure_time[TIME_LENGTH];
} Schedule;

typedef struct {
    char student_id[ID_LENGTH];
    char schedule_id[ID_LENGTH];
    int seat_number;
} Booking;

#endif