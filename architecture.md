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
             ┌──────────────┼──────────────┐
             ▼              ▼              ▼
          STUDENT         DRIVER          ADMIN
             │              │              │
             └──────────────┼──────────────┘
                            ▼
                       CORE MODULES
                            │
              ┌─────────────┼─────────────┐
              ▼             ▼             ▼
         BUS & ROUTE     SCHEDULE    SEAT ALLOCATION
              │             │             │
              └─────────────┼─────────────┘
                            ▼
                     DATA STRUCTURES
                            │
                            ▼
                      FILE HANDLING
                            │
                            ▼
                        DATA FILES
```

## Modules

### Authentication Module
- Login
- Credential validation
- Role identification
- Logout

### Student Module
- Student profile
- Bus and route search
- Schedule viewing
- Seat availability
- Seat allocation and cancellation
- Waiting list

### Driver Module
- Driver profile
- Assigned bus and route
- Daily schedule
- Seat occupancy
- Bus status

### Bus & Route Module
- Bus information and management
- Route and stop management
- Bus/route search
- Bus availability

### Schedule Module
- Schedule management
- Timings
- Full Day / Half Day schedule

### Seat Allocation Module
- Seat availability
- Seat allocation and cancellation
- Seat preferences
- Student-seat mapping
- Waiting list
- Bus occupancy

### Admin Module
- Student management
- Bus management
- Route management
- Driver management
- Bus/route/driver assignment
- Schedule and seat management
- Dashboard

### Reports Module
- Daily Bus Report
- Bus Occupancy Report
- Route-wise Report
- Full Day / Half Day Report
- Driver Report
- Student Seat Report

## GUI Flow

```text
Login
  ↓
Role Selection
  ↓
Student / Driver / Admin Dashboard
  ↓
Required Module
  ↓
Data Processing
  ↓
Data Structures
  ↓
File Handling
```

## Project Structure

```text
BusMate
├── data/
├── documentation/
├── include/
├── src/
├── architecture.md
└── README.md
```