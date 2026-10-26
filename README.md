# BusMate

**Smart College Bus Management System**

BusMate is a C-based graphical application for managing college transportation. It has separate sections for students, drivers, and admins. The system manages buses, routes, schedules, seat allocations, bookings, notifications, and reports using GTK4 and file handling.

## Building

Compile the project from the root directory using:

```bat
build.bat
```

Run on Windows:

```bat
BusMate.exe
```

## How the code is organised

The application is divided into modules, with each module handling a specific part of the system.

```text
                    main.c
                      |
                      v
                    gui.c
                      |
          +-----------+-----------+
          |           |           |
          v           v           v
        Admin       Student      Driver
          |           |           |
          +-----------+-----------+
                      |
       +--------------+--------------+
       |       |       |       |     |
       v       v       v       v     v
      Bus    Route  Schedule Booking Seat Allocation
       |       |       |       |     |
       +-------+-------+-------+-----+
                      |
          +-----------+-----------+
          |           |           |
          v           v           v
     Notification   Report   File Handler
                                  |
                                  v
                               Data Files
```

## Data Storage

The application uses files to store records.

| File                | Purpose                             |
| ------------------- | ----------------------------------- |
| `students.dat`      | Student records                     |
| `admins.dat`        | Admin records                       |
| `drivers.dat`       | Driver records                      |
| `buses.dat`         | Bus details                         |
| `routes.dat`        | Route information                   |
| `schedules.dat`     | Schedule details                    |
| `bookings.dat`      | Booking and seat allocation records |
| `notifications.dat` | Notification records                |
| `reports.dat`       | Report records                      |

## Project Structure

```text
BusMate/
├── include/
├── src/
├── data/
├── documentation/
├── tests/
├── build.bat
└── README.md
```

For details about the system design and implementation, see the `documentation/` directory.
