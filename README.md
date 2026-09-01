# 🚌 BusMate

### Smart College Bus Management & Seat Allocation System

**BusMate** is a GUI-based college transportation management system developed in **C** using **Data Structures and File Handling**. The system provides role-based access for **Students, Drivers, and Administrators**, enabling efficient management of buses, routes, schedules, drivers, and seat allocation.

---

## 📌 Overview

Managing college transportation can become difficult when information about buses, routes, schedules, availability, and seats is scattered or not updated properly.

**BusMate** addresses this problem by providing a centralized system where students can access transportation information, drivers can manage their assigned journeys, and administrators can manage the overall transportation system.

---

## 🎯 Objectives

* Centralize college transportation information.
* Provide role-based access for Students, Drivers, and Admins.
* Allow students to search buses, routes, stops, and timings.
* Provide real-time-in-system seat availability and allocation information.
* Manage Full Day / Half Day bus schedules.
* Allow drivers to update journey status.
* Maintain transportation records using file handling.
* Provide administrators with useful transportation reports.

---

## 👥 User Roles

### 👩‍🎓 Student

Students can:

* Log in using College ID and Password.
* View available buses.
* Search buses and routes.
* View routes, stops, and timings.
* Check Full Day / Half Day status.
* View seat availability.
* Select seat preferences:

  * Window
  * Front
  * Any
* Allocate a seat.
* View their allocated seat.
* Cancel their seat.
* Join the waiting list when seats are unavailable.

### 🚌 Driver

Drivers can:

* Log in to the system.
* View their assigned bus.
* View assigned routes and stops.
* View daily schedules.
* View current seat occupancy.
* Update journey status:

```text
Not Started → Running → Completed
```

### 👨‍💼 Admin

Administrators can:

* Log in through the Admin interface.
* Manage student records.
* Manage buses.
* Manage routes and stops.
* Manage driver records.
* Assign buses, routes, and drivers.
* Manage Full Day / Half Day schedules.
* Manage seat allocation.
* Monitor bus occupancy.
* Generate transportation reports.

---

## 💺 Seat Allocation

The seat management system supports:

* Seat availability checking
* Seat allocation
* Seat cancellation
* Seat preferences
* Automatic seat allocation
* Waiting list management
* Student-seat mapping
* Bus occupancy tracking

---

## 📊 Reports

The Admin module provides:

* **Daily Bus Report**
* **Bus Occupancy Report**
* **Route-wise Report**
* **Full Day / Half Day Report**
* **Driver Report**
* **Student Seat Report**

---

## 🔄 System Workflow

```text
                              ┌───────────────┐
                              │    BusMate    │
                              │  GUI System   │
                              └───────┬───────┘
                                      │
                                  ┌───▼───┐
                                  │ Login │
                                  └───┬───┘
                                      │
                  ┌───────────────────┼───────────────────┐
                  │                   │                   │
                  ▼                   ▼                   ▼
             ┌─────────┐         ┌─────────┐         ┌─────────┐
             │ Student │         │ Driver  │         │  Admin  │
             └────┬────┘         └────┬────┘         └────┬────┘
                  │                   │                   │
                  ▼                   ▼                   ▼
            Bus & Route          Assigned Bus       System Management
            Information           & Schedule         & Reports
                  │                   │                   │
                  └───────────────────┼───────────────────┘
                                      │
                              ┌───────▼────────┐
                              │  Core Modules  │
                              │ Bus / Route    │
                              │ Schedule       │
                              │ Driver         │
                              │ Seat Allocation│
                              └───────┬────────┘
                                      │
                              ┌───────▼────────┐
                              │ Data Structures│
                              │ Search / Sort  │
                              └───────┬────────┘
                                      │
                              ┌───────▼────────┐
                              │ File Handling  │
                              └───────┬────────┘
                                      │
                              ┌───────▼────────┐
                              │   Data Files   │
                              └────────────────┘
```

---

## 🏗️ System Modules

| Module                  | Description                                                                |
| ----------------------- | -------------------------------------------------------------------------- |
| **Admin & Reports**     | Administration, system management, monitoring, and report generation       |
| **Student & Login**     | Authentication, student dashboard, bus/route information, and seat access  |
| **Bus, Route & Driver** | Bus, route, schedule, driver management, and journey status                |
| **Seat Allocation**     | Seat availability, allocation, preferences, cancellation, and waiting list |
| **File Handling**       | Persistent storage and retrieval of system data                            |

---

## 💾 Data Management

BusMate uses **C data structures** to organize application data and **File Handling** to maintain persistent records.

The system stores information related to:

* Students
* Buses
* Routes and stops
* Drivers
* Schedules
* Seat allocations
* Waiting lists

The common data flow is:

```text
User Input
    ↓
GUI
    ↓
Module Logic
    ↓
Data Structures
    ↓
File Handling
    ↓
Data Files
```

---

## 🛠️ Technology Stack

| Technology                | Purpose                                    |
| ------------------------- | ------------------------------------------ |
| **C**                     | Core application development               |
| **GUI Framework/Toolkit** | Graphical user interface                   |
| **Data Structures**       | Efficient data organization and processing |
| **File Handling**         | Persistent data storage                    |

> The specific GUI framework/toolkit will be documented as part of the implementation architecture.

---

## 👥 Team

| Member       | Name         | Module                                  |
| ------------ | ------------ | --------------------------------------- |
| **Member 1** | **Deeya **   | **Admin & Reports**                     |
| **Member 2** | **Smera**    | **Student & Login**                     |
| **Member 3** | **Sapna**    | **Bus, Route & Driver + File Handling** |
| **Member 4** | **Gunjan**   | **Seat Allocation**                     |

---

## 🎯 Project Goal

BusMate aims to make college transportation **organized, accessible, and easier to manage** by providing students, drivers, and administrators with a centralized platform for bus information, scheduling, seat allocation, and transportation management.

---

### 🚌 BusMate

**Smart Transportation. Better Management.**
