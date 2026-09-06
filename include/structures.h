#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    int studentId;
    char name[50];
    char password[30];
} Student;

typedef struct {
    int busId;
    char busNumber[20];
    int capacity;
} Bus;

typedef struct {
    int routeId;
    char routeName[50];
} Route;

typedef struct {
    int driverId;
    char name[50];
} Driver;

#endif