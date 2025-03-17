cmake_minimum_required(VERSION 3.10) #minimun CMake version needed

project(SudokuSolver VERSION 1.0 LANGUAGES CXX)  #project name is SudokuSolver, its version is  1.0, perchance we need to launch more versions, language is cpp (idk why it has to be CXX)

set(CMAKE_CXX_STANDARD 11) #cpp version is 11
set(CMAKE_CXX_STANDARD_REQUIRED True) #must need cpp version 11 atleast

set(SOURCES
    src/main.cpp
    src/generator.cpp
    src/solver.cpp
)                           #our source files, these three will be stored in SOURCES variable predifined by cmake

add_executable(SudokuSolver ${SOURCES})     #executable added by cmake, named SudokuSolver (we can compiler and run all the files with ./SudokuSolver)
                                            #what it does is links the object code of all these three files and makes an executable.
include_directories(${CMAKE_SOURCE_DIR}/src)    #include directory from the source (CMakeLists.txt and the folder src )


add_custom_target(run                           #run target to  run the executable.
    COMMAND "${CMAKE_BINARY_DIR}/SudokuSolver"        
    DEPENDS SudokuSolver
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

)

add_custom_target(custom_clean                  #custom clean target to delete build for a cleaner rebuild, helps in avoiding random bugs with version updates.
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}

)

message(STATUS "Sudoku Solver project configured successfully!")



