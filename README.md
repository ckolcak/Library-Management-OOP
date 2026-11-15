# 📚 Library Management System

A C++-based Library Management System developed to practice **Object-Oriented Programming, relational databases, API integration, and software architecture**.

The project simulates the core operations of a real library system, including book and member management, borrowing and returning books, persistent storage with SQLite, and integration with the Google Books API.

## 🚀 Features

* 📖 Book management

  * Add books using only their ISBN (title, author, and category are fetched automatically)
  * Adding an existing ISBN increases its stock instead of creating a duplicate
  * Track available stock
  * Books that are currently borrowed cannot be removed
* 👤 Member management

  * Add and remove members
  * Store member information (ID, name, phone number, e-mail)
  * Prevent duplicate member IDs
  * Members with an active borrow cannot be removed
* 🔄 Borrow & return system

  * Borrow and return books
  * One active borrow per member
  * Due-date tracking (14 days)
  * Borrow date, due date, and return date are stored for every record
  * Stock and borrow records are always updated together
* 🌐 Google Books API integration

  * Search books using ISBN
  * Retrieve book information from the API
  * Parse JSON responses using `nlohmann/json`
  * Explicit handling of HTTP status codes and automatic retries on temporary errors
* 💾 Data persistence

  * All data is stored in an SQLite database
  * Data is kept between program executions
  * Multi-step operations run inside transactions
* 🏗️ Object-Oriented architecture

  * Separation of responsibilities between classes
  * Encapsulation
  * Composition
  * Classes designed around specific system responsibilities

## 🛠️ Technologies

* **C++17**
* **OOP**
* **SQLite3** — persistent storage
* **libcurl** — HTTP requests
* **nlohmann/json** — JSON parsing
* **Google Books API**
* **Git & GitHub**

## 📂 Project Structure

```text
Library-Management-OOP/
│
├── main.cpp
├── Library.h / Library.cpp
├── DatabaseManager.h / DatabaseManager.cpp
├── BookApiClient.h / BookApiClient.cpp
├── Book.h / Book.cpp
├── Member.h / Member.cpp
├── BorrowRecord.h / BorrowRecord.cpp
├── TimeUtils.h / TimeUtils.cpp
│
└── README.md
```

| File | Responsibility |
|---|---|
| `Library` | Applies the library's rules and shows messages to the user |
| `DatabaseManager` | All reading from and writing to the SQLite database |
| `BookApiClient` | Communication with the Google Books API |
| `Book`, `Member`, `BorrowRecord` | Data models |
| `TimeUtils` | Converting dates to and from text |

## 🧩 Architecture

The `Library` class acts as the central interface of the application.

Instead of requiring the main program or a future GUI to directly manage every individual component, the `Library` class coordinates operations between the database layer and the API client.

```text
                ┌──────────────┐
                │     main     │
                └──────┬───────┘
                       │
                ┌──────▼───────┐
                │   Library    │
                └──┬────────┬──┘
                   │        │
                   ▼        ▼
     ┌─────────────────┐  ┌───────────────┐
     │ DatabaseManager │  │ BookApiClient │
     └────────┬────────┘  └───────┬───────┘
              │                   │
              ▼                   ▼
          SQLite DB        Google Books API
```

Each layer has a single responsibility:

* **`DatabaseManager`** only reads and writes data. It does not make business decisions.
* **`Library`** decides the rules, for example "a member can have only one active borrow" or "a borrowed book cannot be removed".
* **`main`** is the single place where unexpected errors are caught and reported.

Errors are handled in two ways:

* **Expected situations** (book not found, out of stock, member already has a book) return `false` with a clear message.
* **Unexpected errors** (database failure, network failure, broken API response) are thrown as exceptions and reported in `main`.

## 🌐 API Integration

The project uses the **Google Books API** to retrieve book information using ISBN.

The API client is responsible for:

1. Sending HTTP requests using `libcurl`
2. Checking the HTTP status code of the response
3. Retrying when the API is temporarily unavailable or rate-limited
4. Parsing JSON using `nlohmann/json`
5. Converting the received information into a `Book` object

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
Status code check
 │
 ├── 200 → continue
 ├── 429 / 500 / 503 → wait and retry (up to 3 times)
 └── other → error
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

The project uses **SQLite** to store all application data between program executions.

| Table | Columns |
|---|---|
| `Books` | `isbn` (primary key), `title`, `author`, `category`, `stock` |
| `Members` | `id` (primary key), `name`, `number`, `mail` |
| `BorrowRecords` | `recordID` (primary key), `memberID`, `isbn`, `borrowDate`, `dueDate`, `returnDate` |

A borrow record whose `returnDate` is empty is an active borrow.

Important points about the database layer:

* **Transactions:** Operations made of several steps, such as borrowing (decrease stock and create a record) or returning (set the return date and increase stock), run inside a transaction. If one step fails, all steps are undone, so stock and records never get out of sync.
* **Safe stock check:** The stock check is done inside the same SQL statement that decreases the stock. This prevents two borrows from taking the same last copy.
* **Foreign keys:** A borrow record cannot point to a member or book that does not exist.
* **Prepared statements:** User input is never inserted directly into SQL, which protects against SQL injection.

## ⚙️ How to Run (macOS)

Install the dependencies:

```bash
brew install sqlite curl nlohmann-json
```

Set your Google Books API key:

```bash
export GOOGLE_BOOKS_API_KEY="your_api_key"
```

Build:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Book.cpp Member.cpp BorrowRecord.cpp TimeUtils.cpp DatabaseManager.cpp BookApiClient.cpp Library.cpp -I/opt/homebrew/include -I/opt/homebrew/opt/sqlite/include -L/opt/homebrew/opt/sqlite/lib -lcurl -lsqlite3 -o app
```

Run with a clean database:

```bash
rm -f main.db && ./app
```

## 🔮 Planned Improvements

* [x] SQLite database integration
* [x] Better API error handling and retry logic
* [ ] Three user roles: **Admin**, **Librarian**, and **Member**, each with its own permissions
* [ ] Login with username and password
* [ ] Password security: passwords stored only as salted hashes (Argon2), never as plain text
* [ ] REST API so the system can be used over HTTP
* [ ] Improved penalty system for late returns
* [ ] RAII-based resource management for database and network resources
* [ ] Input validation for ISBNs and stored dates

## 🎯 Purpose

This project is primarily a **self-directed learning and portfolio project**.

It was built to move beyond basic C++ syntax and gain practical experience with:

* Object-Oriented Programming
* Class design
* Separation of responsibilities
* Relational databases and SQL
* Transactions and data consistency
* HTTP/API communication
* JSON parsing
* Error handling strategy
* Git/GitHub workflow
* Designing a larger C++ application

## 📌 Current Status

**In development.**

The core library management functionality works end to end with SQLite storage and Google Books API integration. User roles, authentication, and a REST API are the next major development steps.