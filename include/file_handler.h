#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "structures.h"
#include <stdbool.h>

// Existing Team Functions
void saveStudent(Student student);
void saveBus(Bus bus);
void saveRoute(Route route);
void saveDriver(Driver driver);

// ==========================================
// ADDED: Seat Allocation & Waiting List Functions
// ==========================================

#define BUS_DATA_FILE "data/seats.dat"
#define WAITLIST_FILE "data/students.dat"

typedef enum
{
    PREF_ANY = 0,
    PREF_WINDOW,
    PREF_FRONT
} SeatPreference;

bool init_storage_system(void);
bool load_all_seats(void);
bool save_waiting_student(const Student *student);

#endif