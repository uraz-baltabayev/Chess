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
│   └── resources/          ← Create this folder inside src/
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