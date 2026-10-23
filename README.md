# Library Management System (C++)

A console-based Library Management System built with C++17. It demonstrates object-oriented programming, STL containers and algorithms, validation, and file persistence.

## Features

- Add, display, search, update, and delete book records.
- Register and display library members.
- Issue books and record returns.
- Prevent duplicate book/member IDs.
- Track available copies and active loans.
- Assign a 14-day loan period and flag overdue active loans.
- Save records between runs using local text files.
- Validate numeric input and common invalid operations.

## Requirements

- Windows
- GCC / G++ with C++17 support
- VS Code (optional; a task configuration is included)

## Build

From the project directory:

```cmd
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Book.cpp Member.cpp Loan.cpp Library.cpp -o library.exe
```

## Run

```cmd
.\library.exe
```

In VS Code, open this folder, press `Ctrl+Shift+P`, choose `Tasks: Run Task`, then select `Build and Run Library`.

## Data files

The program creates these files in its current working directory:
- `books.txt`
- `members.txt`
- `loans.txt`

Keep these files with the project if you want to retain your local data. They are generated at runtime and do not need to be committed to GitHub if they contain personal/test information.

## Design

- `Book`: book metadata and copy counts.
- `Member`: member details.
- `Loan`: issue/due/return timestamps.
- `Library`: owns collections, performs operations, and saves/loads data.
- `main.cpp`: console menu and input handling.

## Current limitations

This is a local, single-user educational console application. It does not provide authentication, concurrent access, a database server, or fines/payments. The 14-day loan period is fixed in the code. Text files are not encrypted, so use fictional data for demonstrations.
