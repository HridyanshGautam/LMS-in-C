⚠️ Disclaimer: This project is Windows-only and uses Windows-specific libraries (windows.h, conio.h).
->It will not compile on Unix-based systems (Linux/macOS) without major refactoring.
->The system includes self-repair mechanisms that attempt to automatically restore the required file and directory structure if it is missing or tampered with.
->Recovery logic may recreate critical files (e.g., index or credential data) to prevent crashes or undefined behavior.

📂 Project Structure
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