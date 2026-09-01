# 🚌 BusMate

### Smart College Bus Management & Seat Allocation System

**BusMate** is a **GUI-based college transportation management system** developed in **C**, using **Data Structures and File Handling**. It provides role-based access for **Students, Drivers, and Administrators** to manage and access college transportation information including buses, routes, schedules, drivers, and seat allocation.

---

## 📌 Overview

Managing college transportation can become difficult when information about buses, routes, schedules, availability, and seats is scattered or difficult to access.

**BusMate** provides a centralized system where:

* **Students** can access bus information and manage their seats.
* **Drivers** can view assigned buses, routes, schedules, and update journey status.
* **Admins** can manage transportation data, monitor occupancy, and generate reports.
* **File Handling** maintains persistent transportation records.
* **Data Structures** organize and process system information efficiently.

---

## 🎯 Objectives

* Centralize college transportation information.
* Provide role-based access for Students, Drivers, and Admins.
* Allow students to search buses, routes, stops, and timings.
* Provide seat availability and allocation.
* Support Full Day / Half Day bus schedules.
* Allow drivers to update journey status.
* Maintain persistent records using file handling.
* Generate useful transportation reports.
* Integrate all modules into a single working system.

---

# ✨ Features

## 👩‍🎓 Student

* College ID and password login
* Student dashboard
* View available buses
* Search buses and routes
* View routes, stops, and timings
* Check Full Day / Half Day status
* Check seat availability
* Select seat preference:

  * Window
  * Front
  * Any
* Allocate a seat
* View allocated seat
* Cancel allocated seat
* Join waiting list when seats are unavailable

---

## 🚌 Driver

* Driver login
* Driver dashboard
* View assigned bus
* View assigned route and stops
* View daily schedule
* View seat occupancy
* Update bus status:

```text
Not Started → Running → Completed
```

---

## 👨‍💼 Admin

* Admin login and dashboard
* Manage students
* Manage buses
* Manage routes and stops
* Manage drivers
* Assign buses, routes, and drivers
* Manage Full Day / Half Day schedules
* Manage seat allocation
* Monitor bus occupancy
* Generate transportation reports

---

## 💺 Seat Allocation

The seat management system supports:

* Seat availability
* Seat allocation
* Seat cancellation
* Seat preferences
* Automatic seat allocation
* Waiting list management
* Student-seat mapping
* Bus occupancy tracking

---

# 📊 Reports

Administrators can generate:

* **Daily Bus Report**
* **Bus Occupancy Report**
* **Route-wise Report**
* **Full Day / Half Day Report**
* **Driver Report**
* **Student Seat Report**

---

# 🔄 Overall System Workflow

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
           Student Services    Driver Services     Admin Services
                  │                   │                   │
                  └───────────────────┼───────────────────┘
                                      │
                              ┌───────▼────────┐
                              │  Core Modules  │
                              ├────────────────┤
                              │ Bus            │
                              │ Route          │
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

# 🏗️ Core Architecture

BusMate follows a modular architecture that separates the **GUI, application logic, data structures, and persistent storage**.

```text
┌──────────────────────────────────────────┐
│              GUI / UI Layer              │
│ Login │ Student │ Driver │ Admin │ Seats │
└────────────────────┬─────────────────────┘
                     │
┌────────────────────▼─────────────────────┐
│            Application Layer             │
│ Authentication │ Bus │ Route │ Schedule │
│ Driver │ Seat Allocation │ Reports      │
└────────────────────┬─────────────────────┘
                     │
┌────────────────────▼─────────────────────┐
│             Data Layer                   │
│ C Structures │ Searching │ Sorting       │
└────────────────────┬─────────────────────┘
                     │
┌────────────────────▼─────────────────────┐
│           File Handling Layer            │
│ Create │ Read │ Write │ Update │ Delete  │
└────────────────────┬─────────────────────┘
                     │
┌────────────────────▼─────────────────────┐
│              Data Files                  │
└──────────────────────────────────────────┘
```

---

# 🔗 Module Integration

All modules work together through shared data structures and common data storage.

For example:

```text
Admin adds / updates Bus
          ↓
      Bus Data
          ↓
Student can view Bus
          ↓
Seat Module uses Bus Capacity
          ↓
Driver is assigned to Bus
          ↓
Driver updates Bus Status
          ↓
Seat Occupancy is Updated
          ↓
Admin Generates Report
```

This allows information updated by one module to be used by other modules without maintaining separate copies of the same data.

---

# 💾 Data Management & File Handling

BusMate uses **C data structures** for organizing application data and **File Handling** for persistent storage.

The system maintains records related to:

* Students
* Buses
* Routes and stops
* Drivers
* Schedules
* Seat allocations
* Waiting lists

### Data Flow

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

### File Operations

The File Handling module supports:

```text
Create / Open
      ↓
   Read
      ↓
   Write
      ↓
  Update
      ↓
  Delete
      ↓
   Close
```

---

# 🛠️ Technology

| Technology                  | Purpose                                   |
| --------------------------- | ----------------------------------------- |
| **C**                       | Core application development              |
| **GUI Framework / Toolkit** | Graphical user interface                  |
| **Data Structures**         | Data organization and processing          |
| **File Handling**           | Persistent data storage                   |
| **Searching & Sorting**     | Efficient data retrieval and organization |

> The specific GUI framework/toolkit will be documented in the project architecture once finalized.

---

# 👥 Team

| Member       | Name         | Module                                                             |
| ------------ | ------------ | ------------------------------------------------------------------ |
| **Member 1** | **Deeya**    | **Admin, Reports, Core Architecture, Integration & Documentation** |
| **Member 2** | **Smera**    | **Student & Login**                                                |
| **Member 3** | **Sapna**    | **Bus, Route & Driver + File Handling**                            |
| **Member 4** | **Gunjan**   | **Seat Allocation**                                                |

---

# 📚 Project Documentation

The project documentation will cover:

* System Architecture
* Overall Workflow
* Module Design
* Data Structures
* File Handling
* GUI Design
* Module Integration
* Testing
* Project Implementation

Detailed technical documentation will be maintained separately from the README.

---

# 🎯 Project Goal

BusMate aims to make college transportation **organized, accessible, and easier to manage** by providing students, drivers, and administrators with a centralized system for bus information, scheduling, seat allocation, and transportation management.

---

## 🚌 BusMate

**Smart Transportation. Better Management.**
