cmake_minimum_required(VERSION 3.0.0)
project(IntroToCPP)

set(CMAKE_CXX_STANDARD 11) 			# Minimum C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce minimum C++ on all projects

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tell IDE to separate files into folders, instead of one large list
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add libraries
add_subdirectory(raylib)

# Add projects
add_subdirectory(NPCData)
add_subdirectory(TicTacToe)
add_subdirectory(CellularAutomata)
add_subdirectory(HighscoreDatabase)

# Solution filters
set_target_properties(glfw PROPERTIES FOLDER "Third Party")
set_target_properties(raylib PROPERTIES FOLDER "Third Party")
set_target_properties(glfw_objlib PROPERTIES FOLDER "Third Party")