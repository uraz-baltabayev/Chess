<<<<<<< HEAD
chess_c++/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── ChessBoard.cpp
│   ├── ChessBoard.h
│   ├── ChessPiece.cpp
│   ├── ChessPiece.h
│   ├── Game.cpp
│   ├── Game.h
│   └── resources/         
│       ├── wp.png
│       ├── wr.png
│       ├── wn.png
│       ├── wb.png
│       ├── wq.png
│       ├── wk.png
│       ├── bp.png
│       ├── br.png
│       ├── bn.png
│       ├── bb.png
│       ├── bq.png
│       └── bk.png
└── build/


---------------------------
chess c++ compile a program
mkdir build
cd build
cmake ..
make
=======
# Chess (C++)

A simple **2‑player Chess game** written in **C++**, featuring basic game logic and graphics.  
This project demonstrates fundamental game programming concepts including a game loop, rendering, input handling, and object‑oriented design.

---

## 🧠 Features

- Classic Chess gameplay for 1‑on‑1 players
- Chess piece movement and capture rules
- Simple graphical display using image resource files
- Built with **CMake** for easy cross‑platform builds

---

*(The `resources/` folder contains image files for white and black pieces.)* :contentReference[oaicite:1]{index=1}

---

## 🛠️ Build & Run

### 📌 Prerequisites

- C++ compiler (e.g., **g++**, **clang++**, or **MSVC**)
- CMake (to generate build files)

---

### 🔁 Clone the Repository

```bash
git clone https://github.com/uraz-baltabayev/Chess.git
cd Chess

mkdir build
cd build
cmake ..
make

./Chess




## 📦 Project Structure

Chess/
├── CMakeLists.txt # Build configuration
├── src/ # Source code and resources
│ ├── main.cpp # Entry point
│ ├── ChessBoard.cpp / ChessBoard.h
│ ├── ChessPiece.cpp / ChessPiece.h
│ ├── Game.cpp / Game.h
│ └── resources/ # Piece image files
│ ├── wp.png
│ ├── wr.png
│ ├── wn.png
│ ├── wb.png
│ ├── wq.png
│ ├── wk.png
│ ├── bp.png
│ ├── br.png
│ ├── bn.png
│ ├── bb.png
│ ├── bq.png
│ └── bk.png
├── build/ # Generated build files (ignored)
└── README.md # Project documentation

📜 License
This project is open‑source. Feel free to study, modify, and share.
>>>>>>> aafabb3 (updated)
