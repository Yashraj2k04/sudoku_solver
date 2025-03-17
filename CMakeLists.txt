cmake_minimum_required(VERSION 3.10) #minimun CMake version needed

project(SudokuSolver VERSION 1.0 LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/generator.cpp
    src/solver.cpp
)

add_executable(SudokuSolver ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/src)


add_custom_target(run 
    COMMAND "${CMAKE_BINARY_DIR}/SudokuSolver"        
    DEPENDS SudokuSolver
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

)

add_custom_target(custom_clean 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}

)

message(STATUS "Sudoku Solver project configured successfully!")



