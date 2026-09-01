# BusMate File Handling

## Purpose

File handling is used to store BusMate data permanently so that information is available after the program is closed.

## Data Files

```text
data/
├── students.dat
├── drivers.dat
├── buses.dat
├── routes.dat
├── schedules.dat
└── seats.dat
```

## Basic Operations

```text
Program
   ↓
Open File
   ↓
Read / Write / Update
   ↓
Close File
```

### Read

Loads stored records into data structures.

### Write

Stores new records in the corresponding file.

### Update

Modifies existing records.

### Delete

Removes or marks records as inactive where required.

## File Handling Flow

```text
GUI
 ↓
Module
 ↓
Data Structure
 ↓
File Handling
 ↓
Data File
```

## Responsibilities

- Open files
- Read records
- Write records
- Update records
- Maintain persistent data
- Handle file errors
- Close files safely