# BusMate System Architecture

## Main Architecture

```text
                         BUSMATE
                            │
                            ▼
                           GUI
                            │
                            ▼
                          LOGIN
                            │
                            ▼
                     AUTHENTICATION
                            │
                            ▼
                    ROLE IDENTIFICATION
                            │
              ┌─────────────┼─────────────┐
              ▼             ▼             ▼
           STUDENT        DRIVER         ADMIN
              │             │             │
              └─────────────┼─────────────┘
                            ▼
                       CORE MODULES
                            │
             ┌──────────────┼──────────────┐
             ▼              ▼              ▼
        BUS & ROUTE      SCHEDULE    SEAT ALLOCATION
             │              │              │
             └──────────────┼──────────────┘
                            ▼
                     DATA STRUCTURES
                            │
                            ▼
                      FILE HANDLING
                            │
                            ▼
                        DATA FILES
```

## Core Modules

* **Authentication** — Login, validation, role identification, logout
* **Student** — Bus/route search, schedules, seat availability, allocation, waiting list
* **Driver** — Assigned bus/route, schedule, occupancy, bus status
* **Bus & Route** — Bus management, routes, stops, availability
* **Schedule** — Timings, Full Day / Half Day schedules
* **Seat Allocation** — Seat availability, preferences, allocation, cancellation, waiting list
* **Admin** — Student, bus, route, driver and schedule management
* **Reports** — Bus, occupancy, route, schedule, driver and student reports

## Project Structure

```text
BusMate
├── data/
├── documentation/
│   └── architecture.md
├── include/
├── src/
└── README.md
```

## GUI Flow

```text
Login
  ↓
Authentication
  ↓
Role Identification
  ↓
Dashboard
  ↓
Required Module
  ↓
Data Processing
  ↓
File Handling
```
