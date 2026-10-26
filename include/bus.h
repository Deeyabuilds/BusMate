#ifndef BUS_H
#define BUS_H

#include "structures.h"

int add_bus(Bus bus);
int view_buses(void);
int update_bus(Bus bus);
int delete_bus(const char *bus_id);

#endif