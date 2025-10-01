# 🧵 syncarray – A Thread-Safe Array in C

**Author**: Hassan Ali  
**Language**: C (POSIX threads)  
**Purpose**: Demonstrates safe concurrent access to a fixed-size array using mutex synchronization.

---

## 📖 Overview

`syncarray` is a simple thread-safe array implementation in C using `pthread_mutex_t`. It supports basic operations like `put`, `get`, `sum`, and `avg`, all protected by a mutex to ensure correctness in multithreaded environments.

This project includes:
- A thread-safe array module (`syncarray.h`)
- A multithreaded test program (`main.c`) that creates two worker threads inserting 25,000 values each
- Demonstration of `put`, `sum`, and `avg` operations in a concurrent setting

---

## 📂 Files

| File         | Description |
|--------------|-------------|
| `syncarray.h` | Header file defining the `syncarray` structure and function prototypes |
| `syncarray.c` | Source file implementing the syncarray logic (mutex handling, put/get/sum/avg) |
| `main.c`     | Demo program that spawns two threads inserting values into the array |

---

## ⚙️ Features

- ✅ Thread-safe array insertions
- ✅ Safe access to elements (`get`)
- ✅ Aggregate operations (`sum`, `avg`)
- ✅ Mutex-based protection
- ✅ Fixed-size array (50,000 elements)

---

## 🚀 How to Build

```bash
gcc -o syncarray main.c syncarray.c -lpthread