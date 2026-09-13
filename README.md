# 📚 Library Management System

A C++-based Library Management System developed to practice **Object-Oriented Programming, file persistence, API integration, and software architecture**.

The project simulates the core operations of a real library system, including book and member management, borrowing and returning books, and integration with the Google Books API.

## 🚀 Features

* 📖 Book management

  * Add and remove books
  * Track available stock
  * Prevent duplicate books
* 👤 Member management

  * Add and remove members
  * Store member information
* 🔄 Borrow & return system

  * Borrow books
  * Return books
  * Track active borrow records
  * Maximum borrowing limit
  * Due-date tracking
* 🌐 Google Books API integration

  * Search books using ISBN
  * Retrieve book information from the API
  * Parse JSON responses using `nlohmann/json`
* 💾 Data persistence

  * Store books, members, and borrowing records
  * Load data when the application starts
  * Save changes to persistent storage
* 🏗️ Object-Oriented architecture

  * Separation of responsibilities between classes
  * Encapsulation
  * Composition
  * Classes designed around specific system responsibilities

## 🛠️ Technologies

* **C++**
* **OOP**
* **libcurl** — HTTP requests
* **nlohmann/json** — JSON parsing
* **Google Books API**
* **Git & GitHub**

## 📂 Project Structure

```text
Library-Management-OOP/
│
├── Book.h / Book.cpp
├── Member.h / Member.cpp
├── BorrowRecord.h / BorrowRecord.cpp
├── BorrowManager.h / BorrowManager.cpp
├── Library.h / Library.cpp
├── PenaltyRecord.h / PenaltyRecord.cpp
├── PenaltyManager.h / PenaltyManager.cpp
├── BookApiClient.h / BookApiClient.cpp
│
└── README.md
```

## 🧩 Architecture

The `Library` class acts as the central interface of the application.

Instead of requiring the main program or a future GUI to directly manage every individual component, the `Library` class coordinates operations between books, members, and the borrowing system.

For example:

```text
                ┌──────────────┐
                │   Library    │
                └──────┬───────┘
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
      ┌────────┐ ┌──────────┐ ┌──────────────┐
      │  Book  │ │  Member  │ │ BorrowManager│
      └────────┘ └──────────┘ └───────┬──────┘
                                      │
                                      ▼
                                BorrowRecord
```

This structure is intended to keep the application's higher-level logic separate from individual data models and managers.

## 🌐 API Integration

The project uses the **Google Books API** to retrieve book information using ISBN.

The API client is responsible for:

1. Sending HTTP requests using `libcurl`
2. Receiving the API response
3. Parsing JSON using `nlohmann/json`
4. Converting the received information into a `Book` object

The API key is stored as an environment variable rather than directly inside the source code.

```text
ISBN
 │
 ▼
BookApiClient
 │
 ├── libcurl → HTTP Request
 │
 ▼
Google Books API
 │
 ▼
JSON Response
 │
 ▼
nlohmann/json
 │
 ▼
Book Object
```

## 💾 Data Persistence

The current version uses file-based persistence to keep application data between program executions.

The system stores information such as:

* Books
* Members
* Borrow records

The project is planned to transition from text-file storage to **SQLite** as the next major development step.

## 🔮 Planned Improvements

* [ ] Librarian user interface
* [ ] Admin interface
* [ ] SQLite database integration
* [ ] Improved penalty and restriction system
* [ ] Better API error handling and retry logic

## 🎯 Purpose

This project is primarily a **self-directed learning and portfolio project**.

It was built to move beyond basic C++ syntax and gain practical experience with:

* Object-Oriented Programming
* Class design
* Separation of responsibilities
* File I/O and persistence
* HTTP/API communication
* JSON parsing
* Git/GitHub workflow
* Designing a larger C++ application

## 📌 Current Status

**In development.**

The core library management functionality is implemented, while database integration, API robustness, and additional system features are being developed incrementally.
