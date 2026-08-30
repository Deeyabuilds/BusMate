# 🚌 BusMate

### Smart College Bus Management & Seat Allocation System

**BusMate** is a GUI-based college bus management system developed in **C** using **Data Structures and File Handling**. It provides separate interfaces for **Students, Drivers, and Admins** to manage and access college transportation information.

## 🎯 Problem

Students often face difficulty finding available buses, routes, timings, Full Day/Half Day schedules, bus status, and seat availability. Administrators and drivers also need an organized way to manage transportation data.

## 💡 Solution

BusMate centralizes bus, route, driver, schedule, and seat information in one system, making college transportation easier to manage and access.

## ✨ Features

### 👩‍🎓 Student

* College ID & password login
* View available buses
* Search buses and routes
* View stops and timings
* Check Full Day / Half Day status
* Check seat availability
* Select seat preference: **Window / Front / Any**
* Allocate or cancel a seat
* View allocated seat
* Join waiting list when seats are full

### 🚌 Driver

* Driver login
* View assigned bus and route
* View daily schedule
* View seat occupancy
* Update bus status:
  **Not Started → Running → Completed**

### 👨‍💼 Admin

* Admin login & dashboard
* Manage students, buses, routes, and drivers
* Assign buses, routes, and drivers
* Manage schedules and seat allocation
* View bus occupancy
* Generate transportation reports

## 📊 Reports

* Daily Bus Report
* Bus Occupancy Report
* Route-wise Report
* Full Day / Half Day Report
* Driver Report
* Student Seat Report

## 🔄 System Flow

```text
Login
  │
  ├── Student → Buses/Routes → Seats → Allocation
  │
  ├── Driver  → Assigned Bus → Schedule → Status
  │
  └── Admin   → Manage System → Seats → Reports
```

## 🛠️ Technology

* **Language:** C
* **Application:** GUI-based Desktop Application
* **Concepts:** Data Structures & File Handling

## 👥 Project Modules

| Module                  | Responsibility                                         |
| ----------------------- | ------------------------------------------------------ |
| **Student & Login**     | Student access, bus/route information                  |
| **Bus, Route & Driver** | Bus, route, schedule & driver management               |
| **Seat Allocation**     | Seat allocation, preferences, waiting list & occupancy |
| **Admin & Reports**     | Administration, data management & reports              |

## 🎯 Goal

To provide students with clear and updated bus and seat information while making college transportation management **simpler, organized, and efficient**.

---

**BusMate — Making Campus Transit Smarter.**
