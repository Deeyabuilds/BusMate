# 🚌 BusMate

### Smart College Bus Management & Seat Allocation System

Managing college buses usually means dealing with scattered information — which bus goes where, how many seats are left, who is driving it, and whether a student has a seat.

**BusMate** brings these things together in one GUI-based application built in **C**.

Students can check buses, routes and seats, drivers can keep track of their assigned buses, and administrators can manage the overall transport system.

---

## 🚍 What BusMate handles

**For Students**

* Login and personal dashboard
* Search buses and routes
* View stops and timings
* Check available seats
* Choose seat preferences
* Allocate or cancel a seat
* Join the waiting list

**For Drivers**

* Driver login
* View assigned bus and route
* Check schedules and occupancy
* Update bus status

**For Admins**

* Manage students, buses, routes and drivers
* Assign buses, routes and drivers
* Manage schedules and seat information
* Check bus occupancy
* Generate reports

---

## 🪑 The Seat System

Seat allocation is one of the main parts of BusMate.

It keeps track of **which seats are available, which are occupied, and which student has been assigned to each seat**.

When a bus reaches its capacity, students can be added to a **waiting list** instead of losing their request.

```text
Student
   ↓
Select Bus
   ↓
Check Availability
   ↓
Seat Available? ── No ──→ Waiting List
   │
  Yes
   ↓
Allocate Seat
   ↓
Update Occupancy
```

---

## 💾 Where the data goes

BusMate uses **file handling** to keep the application data available between runs.

Instead of every module handling files differently, the project uses a **common file-handling layer** that can be used by Student, Bus, Route, Driver and Seat Allocation modules.

```text
Modules
   ↓
Common File Handling
   ↓
Data Files
```

The shared layer handles operations such as **create, read, write, update, delete and close**.

---

## 🧩 Project Structure

```text
BusMate
│
├── Login & Authentication
├── Student
├── Driver
├── Admin
├── Bus & Route
├── Seat Allocation
├── Reports
├── Data Structures
└── File Handling
```

The overall flow is:

```text
GUI
 ↓
Modules
 ↓
Data Structures
 ↓
File Handling
 ↓
Stored Data
```

---

## 🛠️ Tech Stack

* **C**
* **GUI Framework / Toolkit**
* **Data Structures**
* **File Handling**
* **Searching & Sorting**

---

## 👥 Team

| Member     | Responsibility                                                 |
| ---------- | -------------------------------------------------------------- |
| **Deeya**  | Admin, Reports, Core Architecture, Integration & Documentation |
| **Smera**  | Student Module & Login                                         |
| **Sapna**  | Bus, Route & Driver Modules                                    |
| **Gunjan** | Seat Allocation & File Handling                                |

---

## 📌 Project Focus

The project mainly focuses on applying **C programming and data structures** to a practical college transportation problem, while keeping the system modular enough for different team members to work on and integrate their parts.
