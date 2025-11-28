cmake_minimum_required(VERSION 3.16)
project(ChessGame)

set(CMAKE_CXX_STANDARD 17)

# Find SFML3
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Create executable
add_executable(ChessGame 
    src/main.cpp
    src/ChessBoard.cpp
    src/ChessPiece.cpp
    src/Game.cpp
)

# Copy resources from src/resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_BINARY_DIR})

# Link SFML3
target_link_libraries(ChessGame SFML::Graphics SFML::Window SFML::System)

# Include directories
target_include_directories(ChessGame PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Set properties for macOS
if(APPLE)
    set_target_properties(ChessGame PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()