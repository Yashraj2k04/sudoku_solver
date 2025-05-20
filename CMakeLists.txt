cmake_minimum_required(VERSION 3.10) #minimun CMake version needed

project(SudokuSolver VERSION 1.0 LANGUAGES CXX)  #project name is SudokuSolver, its version is  1.0, perchance we need to launch more versions, language is cpp (idk why it has to be CXX)

set(CMAKE_CXX_STANDARD 17) #cpp version is 17
set(CMAKE_CXX_STANDARD_REQUIRED True) #must need cpp version 11 atleast

set(SOURCES
    ui/main.cpp
    src/generator.cpp
    src/solver.cpp
    ui/main_window.cpp
    ui/main_menu.cpp
    ui/main_controller.cpp
    ui/difficulty.cpp
    ui/login.cpp


)                           #our source files, these three will be stored in SOURCES variable predifined by cmake





find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

qt5_wrap_cpp(MOC_SOURCES 
            ui/main_window.h
            ui/main_menu.h
            ui/main_controller.h
            ui/difficulty.h
            ui/login.h)
                                       #header files associated with the qt widgets that are used anywhere in our code
                                        # threw a lotta errors when i forgot to include the macro (Q_OBJECT is macro or moc) // okay, MOC is meta-object compiler



# NEW addition, we add the qtcreator ui files to cmake build so it recognizes it.


qt5_wrap_ui(UI_HEADERS ${UI_FILES})  # Generates headers from UI files

include_directories(${CMAKE_SOURCE_DIR}/src)    #include directory from the source (CMakeLists.txt and the folder src )
include_directories(${CMAKE_SOURCE_DIR}/ui)     #This included the ui folder in the cmake build consideration



add_executable(SudokuSolver ${SOURCES} ${MOC_SOURCES})     #executable added by cmake, named SudokuSolver (we can compiler and run all the files with ./SudokuSolver)
                                            #what it does is links the object code of all these three files and makes an executable.



### Gotta link qt widget libraries, yeah? 
target_link_libraries(SudokuSolver Qt5::Widgets Qt5::Sql)


add_custom_target(run                           #run target to  run the executable.
    COMMAND "${CMAKE_BINARY_DIR}/SudokuSolver"        
    DEPENDS SudokuSolver
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

)

add_custom_target(custom_clean                  #custom clean target to delete build for a cleaner rebuild, helps in avoiding random bugs with version updates.
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}

)

message(STATUS "Sudoku Solver project configured successfully!")



