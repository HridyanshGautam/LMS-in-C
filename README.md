# LMS-in-C

A console-based **Learning Management System** written entirely in **C**, featuring
user authentication, quiz handling, analytics, and self-repairing file integrity checks.

---

## ⚠️ Disclaimer

- This project is **Windows-only**
- Uses Windows-specific libraries (`windows.h`, `conio.h`)
- **Will not compile on Linux/macOS** without major refactoring
- Includes **self-repair mechanisms** to restore missing or tampered files/folders
- Recovery logic may recreate critical files (index, credentials) to prevent crashes

---

## ✨ Features

- Admin & Student login system
- Quiz creation & evaluation
- Subject-wise analytics
- Persistent file-based storage
- Arrow-key driven console UI
- Automatic directory & index recovery
- First-run setup & version tracking

---

## 📂 Project Structure

```text
LMS-in-C/
│
├── quiz_sys.c                # Main source file
│
├── credentials/              # User & admin credentials
│   ├── admin.txt
│   └── user.txt
│
├── quizzes/                  # Subject-wise quizzes
│   ├── physics/
│   ├── maths/
│   ├── c_prog/
│   ├── linux/
│   └── prob_solving/
│
├── analytics/                # Performance analytics
│   └── subject-wise records
│
├── responses/                # Student quiz responses
│   └── student responses
│
├── index/                    # System indexing & counters
│   ├── index.txt
│   └── user_count.txt
│
└── build.txt                 # First-run & version tracking
```

---

## 🛠️ Build & Run

Compile using a Windows compiler (MinGW / MSVC):

```bash
gcc quiz_sys.c -o lms