# BusMate Data Structures

## Main Structures

### Student

```text
Student
├── studentId
├── name
├── password
├── routeId
└── seatId
```

### Driver

```text
Driver
├── driverId
├── name
├── password
├── busId
└── routeId
```

### Bus

```text
Bus
├── busId
├── busNumber
├── capacity
└── availability
```

### Route

```text
Route
├── routeId
├── routeName
└── stops
```

### Schedule

```text
Schedule
├── scheduleId
├── busId
├── routeId
├── timing
└── dayType
```

### Seat

```text
Seat
├── seatId
├── busId
├── studentId
├── preference
└── status
```

## Data Structures Used

- Structures (`struct`)
- Arrays
- Linked lists where required
- Queues for waiting lists
- Searching and sorting for data management

## Relationships

```text
Student ──→ Seat
Driver  ──→ Bus
Bus     ──→ Route
Bus     ──→ Schedule
Bus     ──→ Seats
Route   ──→ Stops
```